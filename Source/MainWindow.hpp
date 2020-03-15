#ifndef GTKMM_MAINWINDOW_HPP
#define GTKMM_MAINWINDOW_HPP

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

class MainWindow : public Gtk::Window {

public:
  MainWindow();
  virtual ~MainWindow();

protected:
  Gtk::Box mHBoxTitle;
  Gtk::Box mHBoxControls;
  Gtk::Box mVBoxMain;

  Gtk::Label mLabelTitle;
  Gtk::Frame mFrameTitle;

  Gtk::Button mButtonStartTimer;

  Gtk::Label mLabelClock;
  Gtk::Frame mFrameClock;
};

#endif // GTKMM_MAINWINDOW_HPP
