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
    std::vector<std::string> tables = {"CREATE TABLE PROJECTS (NAME TEXT NOT NULL, DESCRIPTION TEXT NOT NULL)",
                                       "CREATE TABLE TRACKS (NAME TEXT NOT NULL, START_TIME REAL)"
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
            std::cout << "time <project-name>";
            return;
        }
        std::string projectName(argv[2]);
        this->startTimeForProject(projectName);
    }
}

void FMController::addProjects(std::string const& name, std::string const& description) {
    Statement stmt;
    stmt.prepare(this->mDB, "INSERT INTO PROJECTS VALUES (?1, ?2)", name, description);
    stmt.execute();
}

void FMController::startTimeForProject(std::string const& name) {
    
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Setting start date and time for " << name << " to " << std::ctime(&t) << std::endl;
    
    Statement stmt;
    stmt.prepare(this->mDB, "INSERT INTO TRACKS VALUES (?1, ?2)", name, (const double)t);
    stmt.execute();
    std::cout << "Done\n";
    /*
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto stop = std::chrono::system_clock::now();
    auto start = std::chrono::system_clock::from_time_t(t);
    auto diff = stop - start;
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(diff).count() << std::endl;
    
    //Statement stmt;
    //stmt.prepare(this->mDB, "INSERT INTO TRACKS VALUES (?1, ?2)", <#Values &&values...#>);*/
}

FMController::~FMController() {
    std::cout << "FMController deconstruction\n";
}
