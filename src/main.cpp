#include "Gui.hpp"

int main(int argc, char **argv) {

Gui gui;

  gui.Create(800, 600);

    gui.Set_Window_Title("CHIP-8 Emulator");

     gui.Execute(argv[0]);

       gui.Destroy();

  return 0;
}
