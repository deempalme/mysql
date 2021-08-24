#ifndef RAMROD_MYSQL_PARAMETER_H
#define RAMROD_MYSQL_PARAMETER_H

#include <cstdint>
#include <istream>
#include <map>
#include <string>

#include<mysql/field_types.h>

#include "ramrod/mysql/param.h"
#include "ramrod/mysql/types.h"

namespace sql { class PreparedStatement; }


namespace ramrod::mysql {
  class param;
  class parameter_metadata;
  class statement;

  class parameter
  {
  public:
    parameter(ramrod::mysql::statement *statement);
    virtual ~parameter();
    /**
     * @brief Binds variables to a prepared statement as parameters
     *
     * Bind variables for the parameter markers in the SQL statement prepared by `prepare()`
     *
     * @param types Defines the types for each variable, the number of variables must be
     *              equal to the number of characters in `types`.
     *                Available types:
     *                  b = bool
     *                  B = blob
     *                  d = double
     *                  f = float
     *                  i = int32
     *                  I = big int
     *                  l = int64
     *                  n = null
     *                  s = string
     *                  t = datetime
     *                  u = uint32
     *                  U = uint64
     * @param variables Variables that will be binded to a parameter, the number of
     *                  variables must be equal to the number of parameters from
     *                  `param_count()`
     *                    Accepted types:
     *                      bool
     *                      std::istream (for: blob)
     *                      double
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
    bool bind_param(const std::string &types, T &...variables);

    void clear_parameters();

    ramrod::mysql::parameter_metadata &get_parameter_metadata();

    int param_count();

    parameter &set_big_int(const unsigned int index, const std::string &value);
    parameter &set_datetime(const unsigned int index, const std::string &value);
    parameter &set_param(const unsigned int index, const bool value);
    parameter &set_param(const unsigned int index, const double value);
    parameter &set_param(const unsigned int index, const float value);
    parameter &set_param(const unsigned int index, const std::int32_t value);
    parameter &set_param(const unsigned int index, const std::int64_t value);
    parameter &set_param(const unsigned int index, std::istream *blob);
    parameter &set_param(const unsigned int index, const std::string &value);
    parameter &set_param(const unsigned int index, const std::uint32_t value);
    parameter &set_param(const unsigned int index, const std::uint64_t value);
    parameter &set_null(const unsigned int index, const int sql_type = MYSQL_TYPE_NULL);

  protected:
    void update_param();
    void update_metadata();
    void update_statement(sql::PreparedStatement *new_statement);

  private:
    void bind_parameter();
    void bind_parameter(bool &value);
    void bind_parameter(double &value);
    void bind_parameter(float &value);
    void bind_parameter(std::int32_t &value);
    void bind_parameter(std::int64_t &value);
    void bind_parameter(std::istream &blob);
    void bind_parameter(const std::string &value);
    void bind_parameter(std::uint32_t &value);
    void bind_parameter(std::uint64_t &value);

    void cast_back(const mysql::index index, const mysql::types type, void *value);

    void error_param(const mysql::error::code code);
    bool exit_param(const char type);
    bool param_set(const unsigned int index);

    sql::PreparedStatement *statement_;
    ramrod::mysql::parameter_metadata *metadata_;

    bool param_error_, param_in_;
    std::string param_types_;
    unsigned int param_counter_;
    int param_total_;
    std::map<const mysql::index, mysql::param> param_vars_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_PARAMETER_H
