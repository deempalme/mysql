#ifndef RAMROD_MYSQL_STATEMENT_H
#define RAMROD_MYSQL_STATEMENT_H

#include <cstdint>
#include <string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "ramrod/mysql/parameter.h"
#include "ramrod/mysql/types.h"


namespace ramrod::mysql {
  class connection;
  class result;
  class result_metadata;

  class statement : public ramrod::mysql::parameter
  {
  public:
    statement(ramrod::mysql::connection *connection);
    statement(ramrod::mysql::connection *connection, const std::string &sql);
    ~statement();

    std::uint64_t affected_rows();

    bool close();

    bool execute();
    bool execute(const std::string &query);
    // TODO:
    ramrod::mysql::result execute_query(const std::string &query);
    int execute_update();
    int execute_update(const std::string &query);

    bool fetch();

    bool free_result();

    ramrod::mysql::result &get_result();

    std::int64_t insert_id();

    std::size_t num_rows();

    bool prepare(const std::string &query);

    void reset();

    operator sql::PreparedStatement*();

  private:
    sql::Connection *connection_;
    sql::PreparedStatement *statement_;
    ramrod::mysql::result *result_;
    ramrod::mysql::result_metadata *metadata_;

    std::uint64_t affected_rows_;
    std::int64_t insert_id_;
    std::size_t num_rows_;
    std::size_t field_count_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_STATEMENT_H
