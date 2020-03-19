#include "FMController.hpp"
#include <iostream>

FMController::FMController() {
  SQLiteDatabase db;
  db.open();
  db.query("hello");
}

FMController::~FMController() {
  std::cout << "FMController deconstruction\n";
}
