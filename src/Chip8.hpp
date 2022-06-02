#pragma once
#include "Typedef.hpp"

class Chip8 {
public:
  Chip8(){};
  ~Chip8(){};

  BYTE m_fontset[80] = { // fontset gpu
          0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
          0x20, 0x60, 0x20, 0x20, 0x70, // 1
          0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
          0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
          0x90, 0x90, 0xF0, 0x10, 0x10, // 4
          0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
          0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
          0xF0, 0x10, 0x20, 0x40, 0x40, // 7
          0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
          0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
          0xF0, 0x90, 0xF0, 0x90, 0x90, // A
          0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
          0xF0, 0x80, 0x80, 0x80, 0xF0, // C
          0xE0, 0x90, 0x90, 0x90, 0xE0, // D
          0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
          0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  std::array<DWORD, 16> m_keymap = { // keymap
          SDLK_m,
          SDLK_RIGHT,
          SDLK_DOWN,
          SDLK_3,
          SDLK_LEFT,
          SDLK_a,
          SDLK_RIGHT,
          SDLK_w,
          SDLK_UP,
          SDLK_d,
          SDLK_n,
          SDLK_c,
          SDLK_z,
          SDLK_x,
          SDLK_f,
          SDLK_v,
  };

  struct INSTRUCTION {  // help for create table for instruction
    WORD opcode;
    WORD mask;
    void (Chip8::*function)();
  };

  std::array<INSTRUCTION, 35> TABLEINSTRUCTION;  // help for create table for instruction


  WORD m_pc; // program counter
  WORD m_i; // register I

  std::array<BYTE, 4096> m_ram{}; // memory ram
  std::array<BYTE, 64 * 32> m_gpu; // gpu
  DWORD m_pixels[64 * 32]; // pixels display



  std::array<BYTE,16> m_v; // register vXXX
  std::array<WORD,16> m_stack; // stack
  std::array<BYTE,16> m_key;
  WORD m_sp; // stack pointer
  WORD m_opcode;
  BYTE m_delay_timer;
  BYTE m_sound_timer;
  BYTE m_status; // register status


  void reset(); // reset all

  BYTE register_x_y(int opcode, int hex, int bit) { return (opcode & hex) >> bit; } // help function set register VX or VY
  
  void execute_instruction();  // set instruction, fetch, decode
  bool loadfile(char* const &FILE); // load file

  bool m_event = true; // Flag to determine event.
  bool playSound = false; // Flag to determine if the sound should be played.
  bool enable_audio = true; // Flag Help Enable / Disable Audio
  bool resume_emulation = true; // Flag Help For Resume/Stop Emulation

private:

  void OP_00E0();   // Clear the screen
  void OP_00EE();   // Return from a subroutine
  void OP_0NNN();   // Execute machine language subroutine at address NNN
  void OP_1NNN();   // Jump to address NNN
  void OP_2NNN();   // Execute subroutine starting at address NNN
  void OP_3XNN();   // Skip the following instruction if the value of register VX equals NN
  void OP_4XNN();   // Skip the following instruction if the value of register VX is not equal to NN
  void OP_5XY0();   // Skip the following instruction if the value of register VX is equal to the value of register VY
  void OP_6XNN();   // Store number NN in register VX
  void OP_7XNN();   // Add the value NN to register VX
  void OP_8XY0();   // Store the value of register VY in register VX
  void OP_8XY1();   // Set VX to VX OR VY
  void OP_8XY2();   // Set VX to VX AND VY
  void OP_8XY3();   // Set VX to VX XOR VY
  void OP_8XY4();   // Add the value of register VY to register VX Set VF to 01 if a carry occurs Set VF to 00 if a carry does not occur
  void OP_8XY5();   // Subtract the value of register VY from register VX Set VF to 00 if a borrow occurs Set VF to 01 if a borrow does not occur
  void OP_8XY6();   // Store the value of register VY shifted right one bit in register VX¹ Set register VF to the least significant bit prior to the shift VY is unchanged
  void OP_8XY7();   // Set register VX to the value of VY minus VX Set VF to 00 if a borrow occurs Set VF to 01 if a borrow does not occur
  void OP_8XYE();   // Store the value of register VY shifted left one bit in register VX¹ Set register VF to the most significant bit prior to the shift VY is unchanged
  void OP_9XY0();   // Skip the following instruction if the value of register VX is not equal to the value of register VY
  void OP_ANNN();   // Store memory address NNN in register I
  void OP_BNNN();   // Jump to address NNN + V0
  void OP_CXNN();   // Set VX to a random number with a mask of NN
  void OP_DXYN();   // Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
  void OP_EX9E();   // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
  void OP_EXA1();   // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
  void OP_FX07();   // Store the current value of the delay timer in register VX
  void OP_FX0A();   // Wait for a keypress and store the result in register VX
  void OP_FX15();   // Set the delay timer to the value of register VX
  void OP_FX18();   // Set the sound timer to the value of register VX
  void OP_FX1E();   // Add the value stored in register VX to register I
  void OP_FX29();   // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
  void OP_FX33();   // Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
  void OP_FX55();   // Store the values of registers V0 to VX inclusive in memory starting at address I I is set to I + X + 1 after operation²
  void OP_FX65();   // Fill registers V0 to VX inclusive with the values stored in memory starting at address I I is set to I + X + 1 after operation²
};
