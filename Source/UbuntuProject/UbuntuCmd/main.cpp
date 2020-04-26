#include <iostream>
#include "../../CrossPlatform/Controller/FMController.hpp"

int main(int argc, const char* argv[]) {
  FMController controller("/home/ambreen/Documents/FocusMeterDB.db");
  if (argc > 1) {
    controller.executeCommand(argc, argv);
  }
  return 0;
}
