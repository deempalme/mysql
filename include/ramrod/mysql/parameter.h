#ifndef RAMROD_MYSQL_PARAMETER_H
#define RAMROD_MYSQL_PARAMETER_H

#include <cstdint>
#include <istream>
#include <map>
#include <string>

#include<mysql/field_types.h>

#include "ramrod/mysql/types.h"

namespace sql { class PreparedStatement; }


namespace ramrod::mysql {
  class param;
  class statement;

  class parameter
  {
  public:
    parameter(ramrod::mysql::statement *statement);
    virtual ~parameter() = 0;
    /**
     * @param types
     *              b = bool
     *              B = blob
     *              d = double
     *              f = float
     *              i = int32
     *              I = big int
     *              l = int64
     *              n = null
     *              s = string
     *              t = datetime
     *              u = uint32
     *              U = uint64
     */
    template<typename ...T>
    bool bind_param(const std::string &types, T &...vars);

    void clear_parameters();

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

    void error_param(const mysql::error::code code);
    bool exit_param();

    bool param_set(const unsigned int index);

    sql::PreparedStatement *statement_;

    bool param_result_, param_in_;
    std::string param_types_;
    unsigned int param_counter_;
    std::map<const mysql::index, mysql::param> param_vars_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_PARAMETER_H
