#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_map>
#include <experimental/filesystem>

#include "../Database/SQLDatabase.hpp"

class FMController {

public:
  enum class State {
                    StartTimerState,
                    TimerRunningState,
                    StopTimerState,
                    AddProjectState,
                    ReadyState
  };

  enum class Trigger {
                      StartTimerTrigger,
                      StopTimerTrigger,
                      AddProjectTrigger
  };

  FMController(std::string const & path_str);
  void executeCommand(int argc, const char *argv[]);
  ~FMController();

protected:
  void create();
  void addProjects(std::vector<std::string> const& arguments);
  void startTimeForProject(std::vector<std::string> const& arguments);
  void stopTimeForProject(std::vector<std::string> const& arguments);
  void showLatestDuration(std::vector<std::string> const& arguments);
  void showTotalTimeSpentTodayForProject(std::vector<std::string> const& arguments);
  void showProjects();

private:
  Connection mDB;
  std::string mPath;
  State currentState;
  std::unordered_map<State, std::vector<std::pair<Trigger, State>>> rules;
  //std::unordered_map<std::string, std::function<bool(std::vector<std::string>)>> commandLineValidator;
};
