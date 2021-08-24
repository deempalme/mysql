#ifndef RAMROD_MYSQL_RESULT_H
#define RAMROD_MYSQL_RESULT_H

#include <cstdint>
#include <iostream>

#include "ramrod/mysql/result_metadata.h"

namespace sql { class ResultSet; }

namespace ramrod::mysql {
  class result_metadata;

  class result
  {
  public:
    result();
    result(sql::ResultSet *result);
    ~result();

    void after_last();
    void before_fisrt();

    bool close();

    std::uint32_t find_column(const std::string &label);

    bool first();

    bool free();

    std::istream *get_blob(const std::uint32_t index) const;
    std::istream *get_blob(const std::string &label) const;
    bool get_bool(const std::uint32_t index) const;
    bool get_bool(const std::string &label) const;
    long double get_double(const std::uint32_t index) const;
    long double get_double(const std::string &label) const;

    std::size_t get_fetch_size();

    std::int32_t get_int(const std::uint32_t index) const;
    std::int32_t get_int(const std::string &label) const;
    std::int64_t get_int64(const std::uint32_t index) const;
    std::int64_t get_int64(const std::string &label) const;

    ramrod::mysql::result_metadata &get_metadata();

    std::uint32_t get_uint(const std::uint32_t index) const;
    std::uint32_t get_uint(const std::string &label) const;
    std::uint64_t get_uint64(const std::uint32_t index) const;
    std::uint64_t get_uint64(const std::string &label) const;

    std::size_t get_row();

    std::string get_string(const std::uint32_t index) const;
    std::string get_string(const std::string &label) const;

    bool is_after_last();
    bool is_before_first();
    bool is_closed();
    bool is_first();
    bool is_last();

    bool is_null(const std::uint32_t index);
    bool is_null(const std::string &label);

    bool last();

    bool next();
    /**
     * @brief Alias of row_count()
     */
    std::size_t num_rows();

    bool previous();

    void refresh_row();

    std::size_t rows_count();

    bool row_deleted();
    bool row_inserted();
    bool row_updated();

    bool was_null();

    ramrod::mysql::result &operator()(sql::ResultSet *result);
    ramrod::mysql::result &operator=(sql::ResultSet *result);
    ramrod::mysql::result &operator=(ramrod::mysql::result &result);
    ramrod::mysql::result *operator=(ramrod::mysql::result *result);
    operator sql::ResultSet*();

  private:
    sql::ResultSet *result_;
    ramrod::mysql::result_metadata metadata_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_RESULT_H
