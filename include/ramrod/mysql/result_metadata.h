#ifndef RAMROD_MYSQL_RESULT_METADATA_H
#define RAMROD_MYSQL_RESULT_METADATA_H

#include <string>

namespace sql {
  class ResultSet;
  class ResultSetMetaData;
}

namespace ramrod::mysql {
  class result_metadata
  {
  public:
    result_metadata();
    result_metadata(sql::ResultSet *result);
    result_metadata(sql::ResultSetMetaData *result);
    ~result_metadata();

    std::string catalog_name(unsigned int column);

    unsigned int column_count();

    std::string column_label(unsigned int column);

    std::string column_name(unsigned int column);

    unsigned int column_display_size(unsigned int column);

    int column_type(unsigned int column);

    std::string column_type_name(unsigned int column);

    std::string column_charset(unsigned int column);

    std::string column_collation(unsigned int column);

    unsigned int precision(unsigned int column);

    unsigned int scale(unsigned int column);

    std::string schema_name(unsigned int column);

    std::string table_name(unsigned int column);

    bool is_auto_increment(unsigned int column);

    bool is_case_sensitive(unsigned int column);

    bool is_currency(unsigned int column);

    bool is_definitely_writable(unsigned int column);

    int is_nullable(unsigned int column);

    bool is_numeric(unsigned int column);

    bool is_read_only(unsigned int column);

    bool is_searchable(unsigned int column);

    bool is_signed(unsigned int column);

    bool is_writable(unsigned int column);

    bool is_zerofill(unsigned int column);

    ramrod::mysql::result_metadata &operator()(sql::ResultSetMetaData *result_metadata);
    ramrod::mysql::result_metadata &operator=(sql::ResultSetMetaData *result_metadata);
    operator sql::ResultSetMetaData*();

  private:
    sql::ResultSetMetaData *meta_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_RESULT_METADATA_H
