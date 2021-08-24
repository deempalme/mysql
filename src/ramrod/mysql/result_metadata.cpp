#include "ramrod/mysql/result_metadata.h"

#include <cppconn/resultset.h>           // for ResultSet
#include <cppconn/resultset_metadata.h>  // for ResultSetMetaData
#include <cppconn/sqlstring.h>           // for SQLString


namespace ramrod::mysql {
  result_metadata::result_metadata() : meta_{nullptr}{}

  result_metadata::result_metadata(sql::ResultSet *result) :
    meta_{nullptr}
  {
    if(result != nullptr) meta_ = result->getMetaData();
  }

  result_metadata::result_metadata(sql::ResultSetMetaData *result) : meta_{result}{}

  result_metadata::~result_metadata(){}

  std::string result_metadata::catalog_name(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getCatalogName(column);
  }

  unsigned int result_metadata::column_count(){
    if(meta_ == nullptr) return 0;
    return meta_->getColumnCount();
  }

  std::string result_metadata::column_label(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getColumnLabel(column);
  }

  std::string result_metadata::column_name(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getColumnName(column);
  }

  unsigned int result_metadata::column_display_size(unsigned int column){
    if(meta_ == nullptr) return 0;
    return meta_->getColumnDisplaySize(column);
  }

  int result_metadata::column_type(unsigned int column){
    if(meta_ == nullptr) return -1;
    return meta_->getColumnType(column);
  }

  std::string result_metadata::column_type_name(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getColumnTypeName(column);
  }

  std::string result_metadata::column_charset(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getColumnCharset(column);
  }

  std::string result_metadata::column_collation(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getColumnCollation(column);
  }

  unsigned int result_metadata::precision(unsigned int column){
    if(meta_ == nullptr) return 0;
    return meta_->getPrecision(column);
  }

  unsigned int result_metadata::scale(unsigned int column){
    if(meta_ == nullptr) return 0;
    return meta_->getScale(column);
  }

  std::string result_metadata::schema_name(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getSchemaName(column);
  }

  std::string result_metadata::table_name(unsigned int column){
    if(meta_ == nullptr) return std::string();
    return meta_->getTableName(column);
  }

  bool result_metadata::is_auto_increment(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isAutoIncrement(column);
  }

  bool result_metadata::is_case_sensitive(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isCaseSensitive(column);
  }

  bool result_metadata::is_currency(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isCurrency(column);
  }

  bool result_metadata::is_definitely_writable(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isDefinitelyWritable(column);
  }

  int result_metadata::is_nullable(unsigned int column){
    if(meta_ == nullptr) return -1;
    return meta_->isNullable(column);
  }

  bool result_metadata::is_numeric(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isNumeric(column);
  }

  bool result_metadata::is_read_only(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isReadOnly(column);
  }

  bool result_metadata::is_searchable(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isSearchable(column);
  }

  bool result_metadata::is_signed(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isSigned(column);
  }

  bool result_metadata::is_writable(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isWritable(column);
  }

  bool result_metadata::is_zerofill(unsigned int column){
    if(meta_ == nullptr) return false;
    return meta_->isZerofill(column);
  }

  result_metadata& result_metadata::operator()(sql::ResultSetMetaData *metadata){
    meta_ = metadata;
    return *this;
  }

  result_metadata& result_metadata::operator=(sql::ResultSetMetaData *metadata){
    meta_ = metadata;
    return *this;
  }

  ramrod::mysql::result_metadata::operator sql::ResultSetMetaData *(){
    return meta_;
  }

} // namespace ramrod::mysql
