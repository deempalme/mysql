#ifndef RAMROD_MYSQL_RESULT_STMT_H
#define RAMROD_MYSQL_RESULT_STMT_H

#include <map>

#include "ramrod/mysql/param.h"
#include "ramrod/mysql/types.h"

namespace sql { class ResultSet; }

namespace ramrod::mysql {
  class result;
  class result_metadata;

  class result_stmt
  {
  public:
    result_stmt(ramrod::mysql::result *result);
    virtual ~result_stmt();

    /**
     *
     * @brief Binds variables to a prepared statement for result storage
     *
     *  Binds columns in the result set to variables.
     *
     *  When fetch() is called to fetch data, the MySQL client/server protocol places the
     *  data for the bound columns into the specified variables var/vars.
     *
     *  A column can be bound or rebound at any time, even after a result set has been
     *  partially retrieved. The new binding takes effect the next time fetch() is called.
     *
     * @param variables Variables that will be binded to a parameter, the number of
     *                  variables must be equal to the number of parameters from
     *                  `field_count()`
     *                    Accepted types:
     *                      bool
     *                      std::istream (for: blob)
     *                      long double
     *                      float
     *                      std::int32_t (for: int and null)
     *                      std::int64_t
     *                      std::uint32_t
     *                      std::uint64_t
     *                      std::string (for: string, datetime, big int)
     *
     * @return `false` if the number of parameters is not the adecuate
     */
    template<typename ...T>
    bool bind_result(T &...variables);

    void clear_results();

    unsigned int field_count();

  protected:
    void update_results();
    void update_metadata(sql::ResultSet *new_result);

  private:
    void bind_results();
    void bind_results(bool &value);
    void bind_results(long double &value);
    void bind_results(float &value);
    void bind_results(std::int32_t &value);
    void bind_results(std::int64_t &value);
    void bind_results(std::istream &blob);
    void bind_results(const std::string &value);
    void bind_results(std::uint32_t &value);
    void bind_results(std::uint64_t &value);

    void cast_back(const mysql::index index, const mysql::types type, void *value);

    void error_result(const mysql::error::code code);
    bool exit_result();

    sql::ResultSet *result_;
    ramrod::mysql::result_metadata *metadata_;

    bool result_error_;
    unsigned int result_counter_;
    unsigned int result_total_;
    std::map<const mysql::index, mysql::param> result_vars_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_RESULT_STMT_H
