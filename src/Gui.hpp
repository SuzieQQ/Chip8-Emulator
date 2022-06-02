#pragma once
#include"Audio.hpp"
#include"Typedef.hpp"
#include"Chip8.hpp"


class Gui {

  public:
       ~Gui();


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
void Init_SDL();
void Create(int h,int w);
void Set_Window_Title(std::string title);
void event(SDL_Event event,std::string OPENFILE);
void Execute(char* const OPENFILE);
void pop_up_load_file();
void Update();
void Destroy();
void info();
void set_gui();
void exit_gui();
void info_gui();
bool mem_reg, _exit , flag_active ,full_screen = false;
bool showmenu = true;
bool load_file = true;

private:

Chip8 chip8;

/// ascii value key
union KeyCode_ASCII {
    const static char M = 0x6D;
    const static char O = 0x6F;
    const static char R = 0x72;
    const static char S = 0x73;
    const static char G = 0x67;
    const static char V = 0x76;
    const static char F = 0x66;
};

KeyCode_ASCII key_ascii;



 void memory_register() {
       static MemoryEditor mem_reg;
       mem_reg.DrawWindow("RAM Register",chip8.m_ram.data(),chip8.m_ram.size(),0);
       //mem_reg.DrawWindow("Memory Register",&chip8.m_ram,chip8.m_ram.size(),0);

 }

 void debug_register () {
     // make some copies (used to determine if value changed so we change color)
     static auto copy_of_v = chip8.m_v;
     static auto copy_of_i = chip8.m_i;
     static auto copy_of_pc = chip8.m_pc;
     static auto copy_of_sp = chip8.m_sp;
     static auto copy_of_stack = chip8.m_stack;
     static auto copy_of_delay_timer = chip8.m_delay_timer;
     static auto copy_of_sound_timer = chip8.m_sound_timer;

     /// RGBA Value (red,green,blue,alpha)
     //auto white = ImVec4(255.0f, 255.0f, 255.0f, 1.0f);
     auto green = ImVec4(0.0f, 255.0f, 0.0f, 1.0f);
     auto red = ImVec4(255.0f, 0.0f, 0.0f, 1.0f);
     auto flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_ReadOnly;

     ImGui::Begin("Registers", nullptr , ImGuiWindowFlags_AlwaysAutoResize);

     ImGui::Indent(10.0f);
     ImGui::AlignTextToFramePadding();
     ImGui::PushItemWidth(30.0f);

     for (int i = 0; i < chip8.m_v.size(); i++) {
         if (i && !(i % 4)) {
         ImGui::NewLine();
         }


         char label[6];
         snprintf(label, sizeof(label), "##v%X", i);
         ImGui::Text(&label[2]);
         ImGui::PushStyleColor(ImGuiCol_Text, copy_of_v[i] == chip8.m_v[i] ? green : red);
         ImGui::SameLine();
         ImGui::InputScalar(label, ImGuiDataType_U8, &chip8.m_v[i], NULL, NULL, "%02X", flags);
         ImGui::SameLine();
         ImGui::PopStyleColor();
     }

     ImGui::NewLine();

     // delay timer
     ImGui::Text("DT");
     ImGui::SameLine();
     ImGui::PushStyleColor(ImGuiCol_Text, chip8.m_delay_timer == chip8.m_delay_timer ? green : red);
     ImGui::InputScalar("##DT", ImGuiDataType_U8, &chip8.m_delay_timer, NULL, NULL, "%02X", flags);
     ImGui::PopStyleColor();
     ImGui::SameLine();

     // sound timer
     ImGui::Text("ST");
     ImGui::SameLine();
     ImGui::PushStyleColor(ImGuiCol_Text, copy_of_sound_timer == chip8.m_sound_timer ? green : red);
     ImGui::InputScalar("##ST", ImGuiDataType_U8, &chip8.m_sound_timer, NULL, NULL, "%02X", flags);
     ImGui::PopStyleColor();
     ImGui::SameLine();

     // stack pointer
     ImGui::Text("SP");
     ImGui::SameLine();
     ImGui::PushStyleColor(ImGuiCol_Text, copy_of_sp == chip8.m_sp ? green : red);
     ImGui::InputScalar("##SP", ImGuiDataType_U8, &chip8.m_sp, NULL, NULL, "%02X", flags);
     ImGui::PopStyleColor();
     ImGui::SameLine();

     // index register
    // ImGui::TextColored(white, " I");
     ImGui::Text(" I");
     ImGui::SameLine();
     ImGui::PushStyleColor(ImGuiCol_Text, copy_of_i == chip8.m_i ? green : red);
     ImGui::InputScalar("##I", ImGuiDataType_U16, &chip8.m_i, NULL, NULL, "%02X", flags);
     ImGui::PopStyleColor();

     // program counter
     //ImGui::TextColored(white, "PC");
     ImGui::Text("PC");
     ImGui::SameLine();
     ImGui::PushStyleColor(ImGuiCol_Text, copy_of_pc == chip8.m_pc ? green : red);
     ImGui::InputScalar("##PC", ImGuiDataType_U16, &chip8.m_pc, NULL, NULL, "%X", flags);
     ImGui::PopStyleColor();

     ImGui::PopItemWidth();
     ImGui::End();

     // update copies
     copy_of_v = chip8.m_v;
     copy_of_i = chip8.m_i;
     copy_of_pc = chip8.m_pc;
     copy_of_sp = chip8.m_sp;
     copy_of_stack = chip8.m_stack;
     copy_of_delay_timer = chip8.m_delay_timer;
     copy_of_sound_timer = chip8.m_sound_timer;
 }

};


