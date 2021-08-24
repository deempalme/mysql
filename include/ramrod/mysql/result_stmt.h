#ifndef RAMROD_MYSQL_RESULT_STMT_H
#define RAMROD_MYSQL_RESULT_STMT_H

#include <map>

#include "ramrod/mysql/param.h"
#include "ramrod/mysql/types.h"

namespace sql { class ResultSet; }

namespace ramrod::mysql {
  class result;

  class result_stmt
  {
  public:
    result_stmt(ramrod::mysql::result *result);
    ~result_stmt();

    template<typename ...T>
    bool bind_result(T &...vars);

    bool clear_results();

  protected:
    void update_results();

  private:
    sql::ResultSet *result_;

    std::map<const mysql::index, mysql::param> result_vars_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_RESULT_STMT_H
