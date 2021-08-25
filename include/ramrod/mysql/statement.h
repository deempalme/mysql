#ifndef RAMROD_MYSQL_STATEMENT_H
#define RAMROD_MYSQL_STATEMENT_H

#include <cstdint>                     // for int64_t
#include <iosfwd>                      // for size_t
#include <string>                      // for string
#include "ramrod/mysql/parameter.h"    // for parameter
#include "ramrod/mysql/result_stmt.h"  // for result_stmt

namespace sql {
  class Connection;
  class PreparedStatement;
}


namespace ramrod::mysql {
  class connection;
  class result;

  class statement : public ramrod::mysql::parameter, public ramrod::mysql::result_stmt
  {
  public:
    statement(ramrod::mysql::connection *connection);
    statement(ramrod::mysql::connection *connection, const std::string &sql);
    ~statement();

    std::size_t affected_rows();

    void after_last();
    void before_first();

    bool close();
    /**
     * @brief Executes a prepared statement
     *
     * Executes previously prepared statement. The statement must be successfully prepared
     * prior to execution, using either the prepare() function, or by passing the second
     * argument to mysql::statement().
     *
     * If the statement is UPDATE, DELETE, or INSERT, the total number of affected rows
     * can be determined by using the affected_rows() function. Likewise, if the query
     * yields a result set the fetch() function is used.
     *
     * All the previous binded results introduced in bind_result() will be removed.
     *
     * @return `true` on success or `false` on failure.
     */
    bool execute();
    /**
     * @brief Executes a new query
     *
     * Executes a new query, all the binded parameters and results are removed.
     *
     * If the statement is UPDATE, DELETE, or INSERT, the total number of affected rows
     * can be determined by using the affected_rows() function. Likewise, if the query
     * yields a result set the fetch() function is used.
     *
     * @return `true` on success or `false` on failure.
     */
    bool execute(const std::string &query);
    /**
     * @brief Executes a separated query
     *
     * Executes a separated query from the statement, the result is returned,
     * see ramrod::mysql::result for more information.
     *
     * @return a mysql::result
     */
    ramrod::mysql::result execute_query(const std::string &query);
    int execute_update();
    int execute_update(const std::string &query);
    /**
     * @brief Fetch results from a prepared statement into the bound variables
     *
     * Fetch the result from a prepared statement into the variables bound by bind_result().
     *
     *   Note:
     *   Note that all columns must be bound by the application before calling fetch().
     *
     * @return `true` in success. Data has been fetched, `false` if there are no more
     *          rows or an error happened
     */
    bool fetch();
    /**
     * @brief Fetches the first row from the prepared statement
     *
     * @return `true` if success. Data has been fetched, `false` if there is no rows or
     *          an error happened
     */
    bool first();

    bool free_result();

    ramrod::mysql::result &get_result();

    std::int64_t insert_id();

    bool is_after_last();
    bool is_before_first();
    bool is_first();
    bool is_last();

    bool last();

    bool next();

    std::size_t num_rows();

    bool prepare(const std::string &query);

    bool previous();

    void reset();

    operator sql::PreparedStatement*();

  private:
    sql::Connection *connection_;
    sql::PreparedStatement *statement_;
    ramrod::mysql::result *result_;

    int affected_rows_;
    std::int64_t insert_id_;
    std::size_t num_rows_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_STATEMENT_H
