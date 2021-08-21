#ifndef RAMROD_MYSQL_RESULT_H
#define RAMROD_MYSQL_RESULT_H

#include <cstdint>
#include <iostream>

namespace sql {
  class Connection;
  class ResultSet;
}

namespace ramrod::mysql {
  class connection;

  class result
  {
  public:
    result(ramrod::mysql::connection *connection);
    ~result();

    void after_last();
    void before_fisrt();

    bool close();
    /**
     * @brief Alias of next()
     */
    bool fetch();

    bool first();

    bool free();

    bool free_result();

    std::istream *get_blob(std::uint32_t index) const;
    std::istream *get_blob(const std::string &label) const;
    bool get_bool(std::uint32_t index) const;
    bool get_bool(const std::string &label) const;
    long double get_double(std::uint32_t index) const;
    long double get_double(const std::string &label) const;
    std::int32_t get_int(std::uint32_t index) const;
    std::int32_t get_int(const std::string &label) const;
    std::int64_t get_int64(std::uint32_t index) const;
    std::int64_t get_int64(const std::string &label) const;
    std::uint32_t get_uint(std::uint32_t index) const;
    std::uint32_t get_uint(const std::string &label) const;
    std::uint64_t get_uint64(std::uint32_t index) const;
    std::uint64_t get_uint64(const std::string &label) const;
    std::string get_string(std::uint32_t index) const;
    std::string get_string(const std::string &label) const;

    bool last();

    bool next();
    /**
     * @brief Alias of row_count()
     */
    std::size_t num_rows();

    bool previous();

    std::size_t row_count();

  private:
    sql::Connection *connection_;
    sql::ResultSet *result_;

    std::uint32_t field_count_;
    std::size_t row_count_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_RESULT_H
