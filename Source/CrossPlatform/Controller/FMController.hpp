#include <iostream>
#include <experimental/filesystem>

#include "../Database/SQLDatabase.hpp"

class FMController {
public:
    enum class State {
        TimerRunning,
        Ready,
        Setup,
        TimerStopped
    };

    enum class Trigger {
        StartedTimer,
        StoppedTimer
    };

    FMController(std::string const & path_str);
    void executeCommand(int argc, const char *argv[]);
    ~FMController();
protected:
    void create();
    void addProjects(std::string const& name, std::string const& project);
    void startTimeForProject(std::string const& name);
private:
    Connection mDB;
    std::string mPath;
};

inline std::ostream& operator<<(std::ostream& os, const FMController::State& s) {
    switch (s) {
        case FMController::State::TimerRunning:
            os << "Timer is running";
            break;
        case FMController::State::Ready:
            os << "Read to start timer";
            break;
        case FMController::State::Setup:
            os << "No projects available, add project";
            break;
        case FMController::State::TimerStopped:
            os << "Stop timer";
        default:
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const FMController::Trigger& t) {
    switch(t) {
        case FMController::Trigger::StartedTimer:
            os << "Timer started";
            break;
        case FMController::Trigger::StoppedTimer:
            os << "Timer stopped";
            break;
        default:
            break;
    }
    return os;
}
