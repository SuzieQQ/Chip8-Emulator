#include "Chip8.hpp"

void Chip8::reset()
{
    // set program counter
    m_pc = 0x200;

    // reset register opcode
    m_opcode = 0;

    // reset register I
    m_i = 0;

    // reset register SP
    m_sp = 0;

    // reset gpu;
    m_gpu.fill(0);

    // reset memory ram;
    m_ram.fill(0);

    // reset stack
    m_stack.fill(0);

    // reset all register v
    //memset(m_v, 0, sizeof(m_v));
     m_v.fill(0);

    // reset key
    m_key.fill(0);

    // load fontset from 0 to 80
    for (int i = 0; i < 80; i++) m_ram[i] = m_fontset[i];

    // reset sound
    m_sound_timer = 0;

    // reset delay
    m_delay_timer = 0;
}

bool Chip8::loadfile(char* const &FILE) {

  reset();  

    std::ifstream romfile(FILE, std::ios::binary | std::ios::ate);

   if (!romfile) {
       std::cout << "not open rom";
        return false;
    }

    std::ifstream::pos_type file_size = romfile.tellg();

    if (file_size == - 1) {
        return false;
    }

   // printf("\033[1;32mLoading ROM: %s\033[0m\n", FILE);

    romfile.seekg(0, std::ios::beg);

    if (!romfile) {
        return false;
    }


         // if (file_size > m_ram.size() - 512){
           //   return false;
          //}


            file_size = m_ram.size() - 512;

    std::vector<BYTE> buffer(file_size);
    romfile.read(reinterpret_cast<char *>(buffer.data()), file_size);

    std::copy(buffer.begin(), buffer.end(), m_ram.begin() + 512);

   return true;
}

void Chip8::OP_00E0() {
 m_gpu.fill(0);
 // for (int i = 0; i < 64 * 32 ; ++i)  m_display[i];
} // Clear the screen

void Chip8::OP_00EE() {

    --m_sp;
    m_pc = m_stack[m_sp];
} // Return from a subroutine


void Chip8::OP_0NNN() {
// nop
};

void Chip8::OP_1NNN() {

m_pc = (m_opcode & 0x0FFF);

} // Jump to address NNN

void Chip8::OP_2NNN() {

m_stack[m_sp] = m_pc;
++m_sp;
m_pc = (m_opcode & 0x0FFF);

}

void Chip8::OP_3XNN() {

if (m_v[register_x_y(m_opcode, 0xF00, 8)] == (m_opcode & 0x00FF))
 m_pc += 2;

} // Skip the following instruction if the value of register VX equals NN

void Chip8::OP_4XNN() {

if (m_v[register_x_y(m_opcode, 0xF00, 8)] != (m_opcode & 0x00FF))
m_pc += 2;

} // Skip the following instruction if the value of register VX is not equal to NN

void Chip8::OP_5XY0() {

if (m_v[register_x_y(m_opcode, 0xF00, 8)] == m_v[register_x_y(m_opcode, 0x0F0, 4)])
m_pc += 2;

} // Skip the following instruction if the value of register VX is equal to the value of register VY

void Chip8::OP_6XNN() {

m_v[register_x_y(m_opcode, 0xF00, 8)] = (m_opcode & 0x00FF);

} // Store number NN in register VX

void Chip8::OP_7XNN() {

m_v[register_x_y(m_opcode, 0xF00, 8)] += (m_opcode & 0x00FF);

} // Add the value NN to register VX

void Chip8::OP_8XY0() {

m_v[register_x_y(m_opcode, 0xF00, 8)] = m_v[register_x_y(m_opcode, 0x0F0, 4)];

} // Store the value of register VY in register VX

void Chip8::OP_8XY1() {

m_v[register_x_y(m_opcode, 0xF00, 8)] |= m_v[register_x_y(m_opcode, 0x0F0, 4)];

} // Set VX to VX OR VY

void Chip8::OP_8XY2() {

m_v[register_x_y(m_opcode, 0xF00, 8)] &= m_v[register_x_y(m_opcode, 0x0F0, 4)];

} // Set VX to VX AND VY

void Chip8::OP_8XY3() {

m_v[register_x_y(m_opcode, 0xF00, 8)] ^= m_v[register_x_y(m_opcode, 0x0F0, 4)];

} // VX to VX XOR VY

void Chip8::OP_8XY4() {

m_v[register_x_y(m_opcode, 0xF00, 8)] += m_v[register_x_y(m_opcode, 0x0F0, 4)];

if (m_v[register_x_y(m_opcode, 0x0F0, 4)] > (0xFF - m_v[register_x_y(m_opcode, 0xF00, 8)]))
 m_v[0xF] = 1;
else
 m_v[0xF] = 0;
} // Add the value of register VY to register VX Set VF to 01 if a carry occurs Set VF to 00 if a carry does not occur

void Chip8::OP_8XY5() {

if (m_v[(m_opcode & 0x00F00) >> 8] > m_v[register_x_y(m_opcode, 0x0F0, 4)])
  m_v[0xF] = 1;
else
  m_v[0xF] = 0;

m_v[register_x_y(m_opcode, 0xF00, 8)] -= m_v[register_x_y(m_opcode, 0x0F0, 4)];
} // Subtract the value of register VY from register VX Set VF to 00 if a borrow occurs Set VF to 01 if a borrow does not occur


void Chip8::OP_8XY6() {

m_v[0xF] = m_v[register_x_y(m_opcode, 0xF00, 8)] & 0x1;
m_v[register_x_y(m_opcode, 0xF00, 8)] >>= 1;

} // Store the value of register VY shifted right one bit in register VX¹ Set register VF to the least significant bit prior to the shift VY is unchanged

void Chip8::OP_8XY7() {

if (m_v[register_x_y(m_opcode, 0xF00, 8)] > m_v[register_x_y(m_opcode, 0x0F0, 4)])
  m_v[0xF] = 0;
else
  m_v[0xF] = 1;

m_v[register_x_y(m_opcode, 0xF00, 8)] = m_v[register_x_y(m_opcode, 0x0F0, 4)] - m_v[register_x_y(m_opcode, 0xF00, 8)];
} // Set register VX to the value of VY minus VX Set VF to 00 if a borrow occurs Set VF to 01 if a borrow does not occur

void Chip8::OP_8XYE() {

m_v[0xF] = m_v[register_x_y(m_opcode, 0xF00, 8)] >> 7;
m_v[register_x_y(m_opcode, 0xF00, 8)] <<= 1;

} // Store the value of register VY shifted left one bit in register VX¹ Set register VF to the most significant bit prior to the shift VY is unchanged

void Chip8::OP_9XY0() {

if (m_v[register_x_y(m_opcode, 0xF00, 8)] != m_v[register_x_y(m_opcode, 0x0F0, 4)])
    m_pc += 2;

} // Skip the following instruction if the value of register VX is not equal to the value of register VY

void Chip8::OP_ANNN() {

m_i = (m_opcode & 0x0FFF); 

} // Store memory address NNN in register I

void Chip8::OP_BNNN() {

m_pc = (m_opcode & 0x0FFF) + m_v[0];

} // Jump to address NNN + V0

void Chip8::OP_CXNN() {

m_v[register_x_y(m_opcode, 0xF00, 8)] = (rand() % (0xFF + 1)) & (m_opcode & 0x00FF);

} // Set VX to a random number with a mask of NN

void Chip8::OP_DXYN() {

DWORD mv_x = m_v[register_x_y(m_opcode, 0xF00, 8)];

DWORD mv_y = m_v[register_x_y(m_opcode, 0x0F0, 4)];

WORD index = 0;

m_v[0xF] = 0;

    for (WORD i = 0; i < (m_opcode & 0x000F); i++)
    {
        DWORD pixel = m_ram[m_i + i];

        for (WORD j = 0; j < 8; j++)
        {
            if ((pixel & (0x80 >> j)) != 0)
            {

                index = ((mv_x + j) + ((mv_y + i) * 64)) % 2048;
                if (m_gpu[index] == 1)
                {
                    //  	SetFlag(C, false);
                    m_v[0xF] = 1;
                }

                m_gpu[index] ^= 1;
            }

            {
                /* code */
            }
        }
    }

} // Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I Set VF to 01 if any set pixels are changed to unset, and 00 otherwise

void Chip8::OP_EX9E() { 

if (m_key[m_v[register_x_y(m_opcode, 0xF00, 8)]] != 0)
m_pc += 2;

} // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed

void Chip8::OP_EXA1() {

if (m_key[m_v[register_x_y(m_opcode, 0xF00, 8)]] == 0)
m_pc += 2;

} // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed

void Chip8::OP_FX07() {

m_v[register_x_y(m_opcode, 0xF00, 8)] = m_delay_timer;

} // Store the current value of the delay timer in register VX

void Chip8::OP_FX0A() {

    bool m_key_pressed = false;

    for (int i = 0; i < 16; i++) {
        if (m_key[i] != 0) {
            m_v[register_x_y(m_opcode, 0xF00, 8)] = i;
            m_key_pressed = true;
        }
    }

    // If no m_key is pressed, return and try again.
    if (!m_key_pressed)
        return;

    m_pc += 2;

} // Wait for a keypress and store the result in register VX

void Chip8::OP_FX15() {

m_delay_timer = m_v[register_x_y(m_opcode, 0xF00, 8)];
} // Set the delay timer to the value of register VX

void Chip8::OP_FX18() {

m_sound_timer = m_v[register_x_y(m_opcode, 0xF00, 8)];

} // Set the sound timer to the value of register VX

void Chip8::OP_FX1E() {

    if (m_i += m_v[register_x_y(m_opcode, 0xF00, 8)] > 0xFFF)
        m_v[0xF] = 1;
    else
        m_v[0xF] = 0;
    m_i += m_v[register_x_y(m_opcode, 0xF00, 8)];

} // Add the value stored in register VX to register I

void Chip8::OP_FX29() {

m_i = m_v[register_x_y(m_opcode, 0xF00, 8)] * 0x5;

} // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX

void Chip8::OP_FX33() {

m_ram[m_i] = m_v[register_x_y(m_opcode, 0xF00, 8)] / 100;
m_ram[m_i + 1] = (m_v[register_x_y(m_opcode, 0xF00, 8)] / 10) % 10;
m_ram[m_i + 2] = m_v[register_x_y(m_opcode, 0xF00, 8)] % 10;

} // Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2

void Chip8::OP_FX55() {

for (int i = 0; i <= (register_x_y(m_opcode, 0xF00, 8)); i++)
    m_ram[m_i + i] = m_v[i];

    // On the original interpreter, when the
    // operation is done, I = I + X + 1.

} // Store the values of registers V0 to VX inclusive in memory starting at address I I is set to I + X + 1 after operation²

void Chip8::OP_FX65() {
    
for (int i = 0; i <= (register_x_y(m_opcode, 0xF00, 8)); i++)
    m_v[i] = m_ram[m_i + i];
    

    // On the original interpreter,
    // when the operation is done, I = I + X + 1.

} // Fill registers V0 to VX inclusive with the values stored in memory starting at address I I is set to I + X + 1 after operation²


void Chip8::execute_instruction() { //

  if(resume_emulation) {

    m_opcode = m_ram[m_pc] << 8 | m_ram[m_pc + 1];
    m_pc += 2;

    // set table instruction
    TABLEINSTRUCTION = {{

        {0x00E0, 0xFFFF, &Chip8::OP_00E0},   // 0x00E0
        {0x00EE, 0xFFFF, &Chip8::OP_00EE},   // 0x00EE
        {0x0000, 0xF000, &Chip8::OP_0NNN},   // 0x0NNN
        {0x1000, 0xF000, &Chip8::OP_1NNN},   // 0x1NNN
        {0x2000, 0xF000, &Chip8::OP_2NNN},   // 0x2NNN
        {0x3000, 0xF000, &Chip8::OP_3XNN},   // 0x3XNN
        {0x4000, 0xF000, &Chip8::OP_4XNN},   // 0x4XNN
        {0x5000, 0xF00F, &Chip8::OP_5XY0},   // 0x5XY0
        {0x6000, 0xF000, &Chip8::OP_6XNN},   // 0x6XNN
        {0x7000, 0xF000, &Chip8::OP_7XNN},   // 0x7XNN
        {0x8000, 0xF00F, &Chip8::OP_8XY0},   // 0x8XY0
        {0x8001, 0xF00F, &Chip8::OP_8XY1},   // 0x8XY1
        {0x8002, 0xF00F, &Chip8::OP_8XY2},   // 0x8XY2
        {0x8003, 0xF00F, &Chip8::OP_8XY3},   // 0x8XY3
        {0x8004, 0xF00F, &Chip8::OP_8XY4},   // 0x8XY4
        {0x8005, 0xF00F, &Chip8::OP_8XY5},   // 0x8XY5
        {0x8006, 0xF00F, &Chip8::OP_8XY6},   // 0x8XY6
        {0x8007, 0xF00F, &Chip8::OP_8XY7},   // 0x8XY7
        {0x800E, 0xF00F, &Chip8::OP_8XYE},   // 0x8XYE
        {0x9000, 0xF00F, &Chip8::OP_9XY0},   // 0x9XY0
        {0xA000, 0xF000, &Chip8::OP_ANNN},   // 0xANNN
        {0xB000, 0xF000, &Chip8::OP_BNNN},   // 0xBNNN
        {0xC000, 0xF000, &Chip8::OP_CXNN},   // 0xCXNN
        {0xD000, 0xF000, &Chip8::OP_DXYN},   // 0xDXYN
        {0xE09E, 0xF0FF, &Chip8::OP_EX9E},   // 0xEX9E
        {0xE0A1, 0xF0FF, &Chip8::OP_EXA1},   // 0xEXA1
        {0xF007, 0xF0FF, &Chip8::OP_FX07},   // 0xFX07
        {0xF00A, 0xF0FF, &Chip8::OP_FX0A},   // 0xFX0A
        {0xF015, 0xF0FF, &Chip8::OP_FX15},   // 0xFX15
        {0xF018, 0xF0FF, &Chip8::OP_FX18},   // 0xFX18
        {0xF01E, 0xF0FF, &Chip8::OP_FX1E},   // 0xFX1E
        {0xF029, 0xF0FF, &Chip8::OP_FX29},   // 0xFX29
        {0xF033, 0xF0FF, &Chip8::OP_FX33},   // 0xFX33
        {0xF055, 0xF0FF, &Chip8::OP_FX55},   // 0xFX55
        {0xF065, 0xF0FF, &Chip8::OP_FX65}}}; // 0xFX65



    // execute instruction
    for (const auto &i : TABLEINSTRUCTION) {

         if (i.opcode == (m_opcode & i.mask)){
           (this->*(i.function))();
            break;
        }
    }




     // write value in to pixels
    for (int i = 0; i < 64 * 32; ++i) {
        m_pixels[i] = (0x00FFFFFF * m_gpu[i]) | 0xFF000000;
    }

    if (m_delay_timer > 0) {
        m_delay_timer--;
    }

  if(enable_audio)
  {
    if (m_sound_timer > 0) {
        if (m_sound_timer == 1)
            playSound = true;
     }
   }
    m_sound_timer--;
 }

} //
