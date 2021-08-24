#ifndef RAMROD_MYSQL_PARAMETER_METADATA_H
#define RAMROD_MYSQL_PARAMETER_METADATA_H

#include <string>  // for string

namespace sql { class ParameterMetaData; }


namespace ramrod::mysql {
  class parameter_metadata
  {
  public:
    parameter_metadata();
    parameter_metadata(sql::ParameterMetaData *result);
    ~parameter_metadata();

    std::string parameter_classname(unsigned int param);

    int parameter_count();

    int parameter_mode(unsigned int param);

    int parameter_type(unsigned int param);

    std::string parameter_typename(unsigned int param);

    int precision(unsigned int param);

    int scale(unsigned int param);

    int is_nullable(unsigned int param);

    bool is_signed(unsigned int param);

    ramrod::mysql::parameter_metadata &operator()(sql::ParameterMetaData *metadata);
    ramrod::mysql::parameter_metadata &operator=(sql::ParameterMetaData *metadata);
    operator sql::ParameterMetaData*();

  private:
    sql::ParameterMetaData *meta_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_PARAMETER_METADATA_H
