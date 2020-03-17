#include "MainWindow.hpp"
#include <gtkmm/application.h>
#include <gtkmm/main.h>
#include <iostream>

int main(int argc, const char* argv[]) {

  auto app = Gtk::Application::create("com.ambreen.focusmeter");
  MainWindow window;
  app->run(window);
  std::cout << "Program ending\n";
  return 0;
}
