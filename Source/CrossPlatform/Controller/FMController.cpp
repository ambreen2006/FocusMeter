#include "FMController.hpp"
#include <iostream>

namespace filesystem = std::experimental::filesystem;

FMController::FMController(std::string const & path_str):
              mPath(filesystem::path(path_str)), mDB(path_str)
{
    bool setup = false;
    if (filesystem::exists(mPath)) {
        std::cout << " already exists" << std::endl;
    } else {
        setup = true;
    }
    
    this->mDB.open();
    if (setup)  this->create();
}

void FMController::create() {
    std::string createStr = "CREATE TABLE PROJECTS (NAME TEXT NOT NULL, DESCRIPTION TEXT NOT NULL) ";
    Statement createStmt;
    createStmt.prepare(this->mDB, createStr.c_str());
    createStmt.execute();
}

FMController::~FMController() {
  std::cout << "FMController deconstruction\n";
}
