#include "FMController.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>

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
    std::vector<std::string> tables = {"CREATE TABLE PROJECTS (NAME TEXT PRIMARY KEY, DESCRIPTION TEXT NOT NULL)",
                                       "CREATE TABLE TRACKS (NAME TEXT NOT NULL, START_TIME INTEGER, END_TIME INTEGER)"
    };

    Statement createStmt;
    for (const auto & creation : tables) {
        createStmt.prepare(this->mDB, creation.c_str());
        createStmt.execute();
    }
}

void FMController::executeCommand(int argc, const char *argv[]) {
    
    std::string command(argv[1]);
    std::cout << "Command: " << command << "\n";

    // Add project
    if (0 == command.compare("add")) {
        if (argc < 4) {
            std::cout << "add <project-name> <project description>\n";
            return;
        }
        std::string projectName(argv[2]);
        std::string projectDescription(argv[3]);
        this->addProjects(projectName, projectDescription);
    } else if (0 == command.compare("time")) {
        if (argc < 3) {
            std::cout << "time <project-name>\n";
            return;
        }
        std::string projectName(argv[2]);
        this->startTimeForProject(projectName);
    } else if (0 == command.compare("stop")) {
      if (argc < 3) {
	std::cout << "stop <project-name>\n";
	return;
      }
      std::string projectName(argv[2]);
      this->stopTimeForProject(projectName);
    } else if (0 == command.compare("latest")) {
      if (argc < 3) {
	std::cout << "latest <project-name>\n";
	return;
      }
      std::string projectName(argv[2]);
      this->showLatestDuration(projectName);
    }
}

void FMController::addProjects(std::string const& name, std::string const& description) {

    Statement stmt;
    stmt.prepare(this->mDB, "INSERT INTO PROJECTS VALUES (?1, ?2)", name, description);
    stmt.execute();
}

void FMController::stopTimeForProject(std::string const& name) {

    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Setting start date and time for " << name << " to " << std::ctime(&t) << std::endl;
    
    Statement stmt;
    stmt.prepare(this->mDB, "UPDATE TRACKS SET END_TIME = ?1 WHERE NAME = ?2", (const long long)t, name);
    stmt.execute();
    std::cout << "Done\n";
}

void FMController::startTimeForProject(std::string const& name) {
   
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Setting start date and time for " << name << " to " << std::ctime(&t) << std::endl;
    
    Statement stmt;
    stmt.prepare(this->mDB, "INSERT INTO TRACKS VALUES (?1, ?2, NULL)", name, (const long long)t);
    stmt.execute();
    std::cout << "Done\n";
}

void FMController::showLatestDuration(std::string const& name) {

  Statement stmt;
  stmt.prepare(this->mDB, "SELECT NAME, MAX(START_TIME), END_TIME FROM TRACKS WHERE NAME = ?1", name);

  for (Row r : stmt) {
    long long  s_time = r.getLong(1);
    long long  e_time = r.getLong(2);

    if (e_time != 0) {
      long long diff = e_time - s_time;
      int factor = 3600;
      std::string metrics = "hours";
      if (diff < factor) {
	factor = 60;
	metrics = "minutes";
      }
 
      std::cout << "Latest time spent " << diff/factor << " " << metrics << "\n";
    }
  }
  std::cout << "Done\n";
}

FMController::~FMController() {
}
