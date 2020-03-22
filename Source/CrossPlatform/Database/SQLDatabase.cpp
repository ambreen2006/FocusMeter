#include "SQLDatabase.hpp"

bool Connection::open() {
    std::cout << "Opening database: " << this->mDBPath.c_str() << "\n";
    int result = sqlite3_open(this->mDBPath.c_str(), this->connection.Set());
    if (result != SQLITE_OK) {
        std::cout << sqlite3_errmsg(this->connection.Get()) << std::endl;
        return result;
    }
    return result;
}

bool Connection::query(const std::string& queryStmt) {
    Statement statement;
    return false;
}

Connection::~Connection() {
    std::cout << "SQLiteDatabase destruction\n";
}
