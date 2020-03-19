#include <sqlite3.h>
#include <iostream>

class SQLiteDatabase {
public:
  bool open();
  bool query(const std::string& queryStmt);
};
