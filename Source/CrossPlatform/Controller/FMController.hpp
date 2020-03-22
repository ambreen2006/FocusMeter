#include <iostream>
#include <experimental/filesystem>

#include "../Database/SQLDatabase.hpp"

class FMController {
public:
  FMController(std::string const & path_str);
  ~FMController();
protected:
    void create();
private:
    Connection mDB;
    std::string mPath;
};
