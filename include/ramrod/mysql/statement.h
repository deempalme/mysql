#ifndef RAMROD_MYSQL_STATEMENT_H
#define RAMROD_MYSQL_STATEMENT_H

#include <cstdint>
#include <string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

namespace ramrod::mysql {
  class connection;
  class result;

  class statement
  {
  public:
    statement(ramrod::mysql::connection *connection);
    ~statement();

    std::uint64_t affected_rows();

    template<typename ...T>
    bool bind_param(const std::string &types, T &...vars);

    template<typename ...T>
    bool bind_result(T &...vars);

    bool close();

    bool execute();
    bool execute(const std::string &query);

    bool fetch();

    bool free_result();

    ramrod::mysql::result get_result();

    std::int64_t insert_id();

    std::uint64_t num_rows();

    bool prepare(const std::string &query);

    void reset();

    ramrod::mysql::statement &bind_parameter(unsigned int index, bool &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, double &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, float &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::int32_t &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::int64_t &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::istream &blob);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::string &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::uint32_t &value);
    ramrod::mysql::statement &bind_parameter(unsigned int index, std::uint64_t &value);

  private:
    void bind_parameter();
    void bind_parameter(bool &value);
    void bind_parameter(double &value);
    void bind_parameter(float &value);
    void bind_parameter(std::int32_t &value);
    void bind_parameter(std::int64_t &value);
    void bind_parameter(std::istream &blob);
    void bind_parameter(std::string &value);
    void bind_parameter(std::uint32_t &value);
    void bind_parameter(std::uint64_t &value);

    void clear_parameters();
    void error_param();
    bool exit_param();

    void set_big_int(unsigned int parameter_index, const std::string &value);
    void set_datetime(unsigned int parameter_index, const std::string &value);
    void set_null(unsigned int parameter_index, int sql_type);

    sql::Connection *connection_;
    sql::PreparedStatement *statement_;

    std::uint64_t affected_rows_;
    std::int64_t insert_id_;
    std::uint64_t num_rows_;

    bool param_result_;
    std::string param_types_;
    unsigned int param_counter_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_STATEMENT_H
