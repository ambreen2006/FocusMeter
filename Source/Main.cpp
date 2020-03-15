#include "MainWindow.hpp"
#include <gtkmm/application.h>

int main(int argc, const char* argv[]) {

  auto app = Gtk::Application::create("com.ambreen.focusmeter");
  MainWindow window;
  return app->run(window);
}
