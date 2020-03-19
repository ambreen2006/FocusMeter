#include "SQLDatabase.hpp"

bool SQLiteDatabase::open() {
  std::cout << "Opening database\n";
  return 0;
}

bool SQLiteDatabase::query(const std::string& queryStmt) {
  std::cout << "Executing query\n";
  return 0;
}
