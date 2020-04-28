#include "FMController.hpp"
#include <string>
#include <chrono>
#include <ctime>
#include <thread>

#include "../Utils/Switch.hpp"

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

    this->rules[State::ReadyState] = {{Trigger::AddProjectTrigger, State::AddProjectState},
				      {Trigger::StartTimerTrigger, State::StartTimerState}};

    this->currentState = State::ReadyState;    
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
  std::vector<std::string> cmd_arguments;

  for (int i = 2; i < argc; i++) {
    cmd_arguments.emplace_back(argv[i]);
  }

  Switch s;
  s.add_case("projects", [&]{ this->showProjects(); });
  s.add_case("add",      [&]{ this->addProjects(cmd_arguments); });
  s.add_case("latest",   [&]{ this->showLatestDuration(cmd_arguments); });
  s.add_case("time",     [&]{ this->startTimeForProject(cmd_arguments); });
  s.add_case("stop",     [&]{ this->stopTimeForProject(cmd_arguments); });
  s.add_case("today",    [&]{ this->showTotalTimeSpentTodayForProject(cmd_arguments); });

  s(command);
}

void FMController::showProjects() {

  Statement stmt;
  stmt.prepare(this->mDB, "SELECT * from PROJECTS");

  for (Row r : stmt) {
    std::cout << "# " << r.getString(0) << " #\n";
    std::cout << "  " << r.getString(1) << "\n\n";
  }

  std::cout << "Done\n";
}

void FMController::addProjects(std::vector<std::string> const& arguments) {
  
  if (arguments.size() < 3) {
    std::cout << "add <project-name> <project description>\n";
    return;
  }

  std::string name = arguments[0];
  std::string description = arguments[1];  
  std::cout << "In add projects\n";
  Statement stmt;
  stmt.prepare(this->mDB, "INSERT INTO PROJECTS VALUES (?1, ?2)", name, description);
  stmt.execute();
}

void FMController::stopTimeForProject(std::vector<std::string> const& arguments) {

  if (arguments.size() < 1) {
    std::cout << "stop <project-name>\n";
    return;
  }

  std::string name = arguments[0];
  auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cout << "Setting end date and time for " << name << " to " << std::ctime(&t) << std::endl;
  
  Statement stmt;
  stmt.prepare(this->mDB, "UPDATE TRACKS SET END_TIME = ?1 WHERE NAME = ?2 and END_TIME = ?3", (const long long)t, name, 0);
  stmt.execute();
  std::cout << "Done\n";
}

void FMController::startTimeForProject(std::vector<std::string> const& arguments) {

  if (arguments.size() < 1) {
    std::cout << "time <project-name>\n";
    return;
  }

  std::string name = arguments[0];
  auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cout << "Setting start date and time for " << name << " to " << std::ctime(&t) << std::endl;
  
  Statement stmt;
  stmt.prepare(this->mDB, "INSERT INTO TRACKS VALUES (?1, ?2, 0)", name, (const long long)t);
  stmt.execute();
  std::cout << "Done\n";
}

void FMController::showLatestDuration(std::vector<std::string> const& arguments) {

  if (arguments.size() < 1) {
    std::cout << "latest <project-name>\n";
    return;
  }
                        
  std::string name = arguments[0];
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

void FMController::showTotalTimeSpentTodayForProject(std::vector<std::string> const& arguments) {

  if (arguments.size() < 1) {
    std::cout << "today <project-name>\n";
    return;
  }

  std::string name = arguments[0];
  auto rightnow = std::chrono::system_clock::now();
  auto rightnow_t = std::chrono::system_clock::to_time_t(rightnow);
  auto ltime = std::localtime(&rightnow_t);
  ltime->tm_hour = 0;
  ltime->tm_min = 0;
  ltime->tm_sec = 0;
  auto midnight = std::chrono::system_clock::from_time_t(std::mktime(ltime));
  auto midnight_t = std::chrono::system_clock::to_time_t(midnight);

  std::cout << "Since " << std::mktime(ltime) << std::endl;
  std::cout << "Midnight " << midnight_t << std::endl;
 
  Statement stmt;
  stmt.prepare(this->mDB, "SELECT NAME, START_TIME, END_TIME FROM TRACKS WHERE NAME = ?1", name);

  std::cout << "Today's log for " << name << std::endl;
  for (Row r : stmt) {

    time_t end = r.getLong(2);
    time_t start = r.getLong(1);
    
    if (start > midnight_t) {
      auto start_tp = std::chrono::system_clock::from_time_t(start);
      auto end_tp = std::chrono::system_clock::from_time_t(end);
      auto diff_mins = std::chrono::duration_cast<std::chrono::minutes>(end_tp - start_tp);
      std::cout << "        Duration " << diff_mins.count() << " Minutes START " << start  << " END " <<  end << std::endl;
    }
  }
}

FMController::~FMController() {
}
