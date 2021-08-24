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

    bool execute();
    bool execute(const std::string &query);
    // TODO:
    ramrod::mysql::result execute_query(const std::string &query);
    int execute_update();
    int execute_update(const std::string &query);

    bool fetch();

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
