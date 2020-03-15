#include "MainWindow.hpp"

MainWindow::MainWindow() {

  set_default_size(500, 300);

  this->mHBoxTitle = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
  this->mFrameTitle = Gtk::Frame();
  this->mLabelTitle = Gtk::Label("Zoom in your work with FocusMeter");
  this->mFrameTitle.property_shadow_type() = Gtk::SHADOW_NONE;
  this->mFrameTitle.add(this->mLabelTitle);
  this->mHBoxTitle.set_center_widget(this->mFrameTitle);
  
  this->mHBoxControls = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
  this->mButtonStartTimer = Gtk::Button("Start Work");
  this->mButtonStartTimer.set_size_request(80, 32);
  this->mHBoxControls.set_center_widget(this->mButtonStartTimer);
  this->mHBoxControls.set_spacing(10);

  this->mLabelClock = Gtk::Label();
  this->mLabelClock.set_markup("<span font_desc='Sans Bold 50'>30 mins </span>");
  this->mFrameClock = Gtk::Frame();
  this->mFrameClock.property_shadow_type() = Gtk::SHADOW_NONE;
  this->mFrameClock.add(this->mLabelClock);

  this->mVBoxMain = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
  this->mVBoxMain.pack_start(this->mHBoxTitle, false, true, 10);
  this->mVBoxMain.pack_start(this->mFrameClock);
  this->mVBoxMain.pack_start(this->mHBoxControls, false, false, 10);

  add(this->mVBoxMain);
  
  show_all_children();  
}

MainWindow::~MainWindow() {
}
