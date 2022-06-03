#include"Gui.hpp"

Gui::~Gui()  {
    Destroy();
}

void Gui::Init_SDL() {
 SDL_Init(SDL_INIT_EVERYTHING);

if (SDL_INIT_EVERYTHING < 0) {
   SDL_Error(SDL_LASTERROR);
   }
}


void Gui::Create(int w,int h)  {

Init_SDL();

window = SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

if (window==NULL) {
SDL_Error(SDL_LASTERROR);
} else {
printf("\n\t\t\t\033[1;32m CHIP-8 Emulator \033[0m\n");
}

renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

if (renderer==NULL) {
SDL_Error(SDL_LASTERROR);
}


ImGui::CreateContext();
ImGuiSDL::Initialize(renderer, w, h);


texture =  SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,64,32);

if (texture==NULL){
SDL_Error(SDL_LASTERROR);
}

}


void Gui::Set_Window_Title(std::string title) {
   SDL_SetWindowTitle(window,title.c_str());
}


void Gui::Update() {

ImGui::Render();
ImGuiSDL::Render(ImGui::GetDrawData());
SDL_SetRenderTarget(renderer, texture);
SDL_UpdateTexture(texture,NULL,chip8.m_pixels,64 * sizeof(DWORD));
SDL_RenderPresent(renderer);
SDL_RenderClear(renderer);
SDL_RenderCopy(renderer,texture,NULL,NULL);
SDL_Delay(8);
}


void Gui::exit_gui() {
    exit(0);
}


void Gui::info_gui() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Info ","CHIP-8 Emulator By Suzie Q",window);
}


void Gui::set_gui() {

  if (ImGui::BeginMainMenuBar()) {
      if(ImGui::BeginMenu("File")){
          if(ImGui::MenuItem("Load ROM", "CTRL+O")) {

              char file[1024];

   FILE* fp = popen("zenity --file-selection --file-filter='CH8 files (ch8) | *.ch8' "
                     "--file-filter=' (All Files) | *   '"
                     "--title=Open' Game '" ,"r");
             fgets(file, sizeof(file), fp);
               if (file[strlen(file) - 1] == '\n') {
                   file[strlen(file) - 1] = 0;    
               }
                chip8.loadfile(file);
   }

        ImGui::MenuItem("Exit","ALT + F4",&_exit);
        ImGui::EndMenu();
          }
       }

       if (ImGui::BeginMenu("Debug")) {
        ImGui::MenuItem("Registers","CTRL+R",&flag_active);
        ImGui::MenuItem("RAM Register","CTRL+G",&mem_reg);
        ImGui::MenuItem("Resume | Stop Emulation","CTRL+S",&chip8.resume_emulation);
        ImGui::MenuItem("Disable | Active Menu","CTRL+M",&showmenu);
        ImGui::EndMenu();
}

       if(ImGui::BeginMenu("Video")){
         ImGui::MenuItem("FullScreen Disable | Active","ALT+ENTER",&full_screen);
         ImGui::EndMenu();
       }


       if(ImGui::BeginMenu("Audio")){
         ImGui::MenuItem("Disable | Active","CTRL+V",&chip8.enable_audio);
         ImGui::EndMenu();
       }


      if(ImGui::MenuItem("Info")) {
            info_gui();
       }

       ImGui::EndMainMenuBar();

      if(_exit) {
        exit_gui();
       }

    if(flag_active) {
        debug_register();
      }

      if(mem_reg) {
          memory_register();
      }

   if(full_screen) {
       SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
   } else {
       SDL_SetWindowFullscreen(window,0);
   }
}



void Gui::pop_up_load_file(){

        char file[1024];
         FILE* fp = popen("zenity --file-selection --file-filter='CH8 files (ch8) | *.ch8' "
                    "--file-filter=' (All Files) | *   '"
                    "--title=Open' Game '" ,"r");
                fgets(file, sizeof(file), fp);
                  if (file[strlen(file) - 1] == '\n') {
                      file[strlen(file) - 1] = 0;
                      chip8.loadfile(file);
               }
}





void Gui::Execute(char* const OPENFILE) {

SDL_Event event;

  Audio::open();

  int wheel;

 while (chip8.m_event) {

  SDL_PollEvent(&event);

   chip8.execute_instruction();

   ImGuiIO& io = ImGui::GetIO();


   if(event.type==SDL_QUIT) {
            exit(0);
     }


   for (int i = 0; i < 16; ++i) {
     if (event.key.keysym.sym == chip8.m_keymap[i]) {
       chip8.m_key[i] = 1;
     }
   }

    if (event.key.type==SDL_KEYUP) {
       for (int i = 0; i < 16; ++i)  {
         if (event.key.keysym.sym == chip8.m_keymap[i]) {
           chip8.m_key[i] = 0;
          }
       }
    }

  if(event.type==SDL_DROPFILE) {

    char *FILEDROP = event.drop.file;

    if (chip8.loadfile(FILEDROP)) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"LOAD NEW GAME CHIP-8 ",FILEDROP,window);
      SDL_free(FILEDROP);
      }
    }


    if (chip8.playSound == true) {
                Audio::Beep();
               chip8.playSound = false;
            }

if (event.type == SDL_KEYDOWN) {
      if (event.type == SDL_WINDOWEVENT) {
                 if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                     io.DisplaySize.x = static_cast<float>(event.window.data1);
                     io.DisplaySize.y = static_cast<float>(event.window.data2);
                 }
             } else if (event.type == SDL_MOUSEWHEEL) {
                 wheel = event.wheel.y;
}


if((event.key.keysym.sym) == key_ascii.R && ((event.key.keysym.mod & KMOD_LCTRL))) {
    if(flag_active == true) { flag_active = false; }
                        else if (flag_active == false) { flag_active = true;}
}


if((event.key.keysym.sym) == key_ascii.S && ((event.key.keysym.mod & KMOD_LCTRL))) {
    if(chip8.resume_emulation == true) { chip8.resume_emulation = false; }
                            else if (chip8.resume_emulation == false) { chip8.resume_emulation = true; }
}


if((event.key.keysym.sym) == key_ascii.M && ((event.key.keysym.mod & KMOD_LCTRL))) {
    if(showmenu == true) { showmenu = false; }
                            else if (showmenu == false) { showmenu = true; }
}


if((event.key.keysym.sym) == key_ascii.O && ((event.key.keysym.mod & KMOD_LCTRL))) {
    pop_up_load_file();
}


if((event.key.keysym.sym) == key_ascii.G && ((event.key.keysym.mod & KMOD_LCTRL))) {
    if(mem_reg == true) { mem_reg = false; }
                           else if (mem_reg == false) { mem_reg = true; }
}


if((event.key.keysym.sym) == key_ascii.ENTER && ((event.key.keysym.mod & KMOD_LALT))) {
    if(full_screen == true) { full_screen = false; }
                            else if (full_screen == false) { full_screen = true; }
}


if((event.key.keysym.sym) == key_ascii.V && ((event.key.keysym.mod & KMOD_LCTRL)))
{
    if(chip8.enable_audio == true) { chip8.enable_audio = false; }
                           else if (chip8.enable_audio == false) { chip8.enable_audio = true;}

   }
 }



 int mouseX, mouseY;
 const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

 // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

 io.DeltaTime = 1.0f / 60.0f;
 io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
 io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
 io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
 io.MouseWheel = static_cast<float>(wheel);

 ImGui::NewFrame();


#ifdef NDEBUG
 ImGui::End(); // disabled in debug mode because the emulator crashes when compiled in debug mode
#endif


   if(showmenu) {
      set_gui();
}

  Update();

  }
}




void Gui::Destroy() {
    //ImGuiSDL::Deinitialize();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //ImGui::DestroyContext();
}

