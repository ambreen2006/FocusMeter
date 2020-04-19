#include <sqlite3.h>
#include <iostream>
#include "../Utils/ResourceHandler.hpp"

class Connection {

    struct ConnectionHandlerTraits : ResourceHandleTraits<sqlite3 *> {
        static void Close(Type value) noexcept {
            std::cout << "Closing sqlite3 connection" << std::endl;
            sqlite3_close(value);
        }
    };

    using ConnectionHandle = ResourceHandle<ConnectionHandlerTraits>;

public:
    Connection(const std::string dbPath) : mDBPath(dbPath) {};
    ~Connection();
    bool open();
    bool query(const std::string& queryStmt);
    sqlite3 * Get() const noexcept {
        return connection.Get();
    }
protected:
    std::string mDBPath;
private:
    ConnectionHandle connection;
};

template <typename T>
struct Reader {
    int getInt(int const column = 0) const noexcept {
        return sqlite3_column_int(static_cast<T const *>(this)->Get(), column);
    }

    char const * getString(int const column = 0) const noexcept {
        return reinterpret_cast<char const *>(sqlite3_column_text(static_cast<T const *>(this)->Get(), column));
    }
};

class Statement : public Reader<Statement> {
    struct StatementHandlerTraits : ResourceHandleTraits<sqlite3_stmt *>{
        static void Close(Type value) {
            sqlite3_finalize(value);
        }
    };

    using StatementHandler = ResourceHandle<StatementHandlerTraits>;
    StatementHandler mHandle;

    template <typename F, typename C, typename ... Values>
    void internalPrepare(Connection const& connection,
                         F prepare,
                         C const * const text,
                         Values && ... values) {
        int result = prepare(connection.Get(), text, -1, mHandle.Set(), nullptr);
        if (result != SQLITE_OK) {
            std::cout << "Error preparing statement " << std::endl;
        }
        bindAll(std::forward<Values>(values) ...);
    }

    void internalBind(int) const noexcept
    {}

    template <typename First, typename ... Rest>
    void internalBind(int const index, First && first, Rest && ... rest) const
    {
        bind(index, std::forward<First>(first));
        internalBind(index + 1, std::forward<Rest>(rest) ...);
    }

public:
    Statement() noexcept = default;

    template <typename C, typename ... Values>
    Statement(Connection const & connection,
              C const * const text,
              Values && ... values) {
        this->prepare(connection, text, std::forward<Values>(values) ...);
    }

    explicit operator bool() const noexcept {
        return true;
    }

    template <typename ... Values>
    void prepare(Connection const& connection,
                 char const * const text,
                 Values && ... values) {
        this->internalPrepare(connection, sqlite3_prepare_v2, text, std::forward<Values>(values) ...);
    }

    sqlite3_stmt * Get() const noexcept {
        return mHandle.Get();
    }

    bool step() const {
        int const result = sqlite3_step(mHandle.Get());
        if (result == SQLITE_ROW) return true;
        return false;
    }

    void execute() const {
        step();
    }

    void bind(int const index, int const value) const {
        if (SQLITE_OK != sqlite3_bind_int(Get(), index, value)) {
            std::cout << "Error binding int\n";
        }
    }
    
    void Bind(int const index, double const value) const
     {
         if (SQLITE_OK != sqlite3_bind_double(Get(), index, value))
         {
             std::cout << "Error binding double\n";
         }
     }

    void bind(int const index, char const * const value, int size = -1) const {
        if (SQLITE_OK != sqlite3_bind_text(Get(), index, value, size, SQLITE_TRANSIENT)) {
            std::cout << "Error binding text\n";
        }
    }

    void bind(int const index, std::string const & value) const {
        bind(index, value.c_str(), (int)value.size());
    }

    void bind(int const index, std::string && value) const {
        bind(index, value.c_str(), (int)value.size());
    }

    template <typename ... Values>
    void bindAll(Values && ... values) {
        internalBind(1, std::forward<Values>(values) ...);
    }
};

class Row : public Reader<Row> {
    sqlite3_stmt * m_statement = nullptr;
public:
    sqlite3_stmt * Get() const noexcept {
        return m_statement;
    }

    Row(sqlite3_stmt * const statement) noexcept : m_statement(statement) {}
};

class RowIterator {
    Statement const * m_statement = nullptr;
public:
    RowIterator() noexcept = default;
    RowIterator(Statement const & statement) noexcept {
        if (statement.step()) {
            m_statement = &statement;
        }
    }

    RowIterator & operator++() noexcept {
        if (!m_statement->step()) {
            m_statement = nullptr;
        }
        return *this;
    }

    bool operator!=(RowIterator const & other) const noexcept {
        return m_statement != other.m_statement;
    }

    Row const operator *() const noexcept {
        return Row(m_statement->Get());
    }
};

inline RowIterator begin(Statement const & statement) noexcept {
    return RowIterator(statement);
}

inline RowIterator end(Statement const &) noexcept {
    return RowIterator();
}
