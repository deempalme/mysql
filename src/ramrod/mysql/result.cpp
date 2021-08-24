#include "ramrod/mysql/result.h"

#include <mysql_connection.h>
#include <cppconn/resultset.h>


namespace ramrod::mysql {
  result::result() :
    result_{nullptr},
    metadata_()
  {}

  result::result(sql::ResultSet *result) :
    result_{result},
    metadata_(result)
  {}

  result::~result(){
    close();
  }

  void result::after_last(){
    if(result_ == nullptr) return;
    result_->afterLast();
  }

  void result::before_fisrt(){
    if(result_ == nullptr) return;
    result_->beforeFirst();
  }

  bool result::close(){
    if(result_ == nullptr) return false;
    metadata_(nullptr);
    delete result_;
    result_ = nullptr;
    return true;
  }

  std::uint32_t result::find_column(const std::string &label){
    if(result_ == nullptr) return 0;
    return result_->findColumn(label);
  }

  bool result::first(){
    if(result_ == nullptr) return false;
    return result_->first();
  }

  bool result::free(){
    return close();
  }

  std::istream *result::get_blob(const std::uint32_t index) const {
    if(result_ == nullptr) return nullptr;
    return result_->getBlob(index);
  }

  std::istream *result::get_blob(const std::string &label) const {
    if(result_ == nullptr) return nullptr;
    return result_->getBlob(label);
  }

  bool result::get_bool(const std::uint32_t index) const {
    if(result_ == nullptr) return false;
    return result_->getBoolean(index);
  }

  bool result::get_bool(const std::string &label) const {
    if(result_ == nullptr) return false;
    return result_->getBoolean(label);
  }

  long double result::get_double(const std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getDouble(index);
  }

  long double result::get_double(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getDouble(label);
  }

  std::size_t result::get_fetch_size(){
    if(result_ == nullptr) return 0;
    return result_->getFetchSize();
  }

  std::int32_t result::get_int(const std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getInt(index);
  }

  std::int32_t result::get_int(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getInt(label);
  }

  std::int64_t result::get_int64(const std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getInt64(index);
  }

  std::int64_t result::get_int64(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getInt64(label);
  }

  ramrod::mysql::result_metadata &result::get_metadata(){
    return metadata_;
  }

  std::uint32_t result::get_uint(const std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt(index);
  }

  std::uint32_t result::get_uint(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt(label);
  }

  std::uint64_t result::get_uint64(const std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt64(index);
  }

  std::uint64_t result::get_uint64(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt64(label);
  }

  std::size_t result::get_row(){
    if(result_ == nullptr) return 0;
    return result_->getRow();
  }

  std::string result::get_string(const std::uint32_t index) const {
    if(result_ == nullptr) return std::string();
    return result_->getString(index);
  }

  std::string result::get_string(const std::string &label) const {
    if(result_ == nullptr) return std::string();
    return result_->getString(label);
  }

  bool result::is_after_last(){
    if(result_ == nullptr) return false;
    return result_->isAfterLast();
  }

  bool result::is_before_first(){
    if(result_ == nullptr) return false;
    return result_->isBeforeFirst();
  }

  bool result::is_closed(){
    if(result_ == nullptr) return false;
    return result_->isClosed();
  }

  bool result::is_first(){
    if(result_ == nullptr) return false;
    return result_->isFirst();
  }

  bool result::is_last(){
    if(result_ == nullptr) return false;
    return result_->isLast();
  }

  bool result::is_null(const std::uint32_t index){
    if(result_ == nullptr) return false;
    return result_->isNull(index);
  }

  bool result::is_null(const std::string &label){
    if(result_ == nullptr) return false;
    return result_->isNull(label);
  }

  bool result::last(){
    if(result_ == nullptr) return false;
    return result_->last();
  }

  bool result::next(){
    if(result_ == nullptr) return false;
    return result_->next();
  }

  std::size_t result::num_rows(){
    return rows_count();
  }

  bool result::previous(){
    if(result_ == nullptr) return false;
    return result_->previous();
  }

  void result::refresh_row(){
    if(result_ == nullptr) return;
    result_->refreshRow();
  }

  std::size_t result::rows_count(){
    if(result_ == nullptr) return 0;
    return result_->rowsCount();
  }

  bool result::row_deleted(){
    if(result_ == nullptr) return false;
    return result_->rowDeleted();
  }

  bool result::row_inserted(){
    if(result_ == nullptr) return false;
    return result_->rowInserted();
  }

  bool result::row_updated(){
    if(result_ == nullptr) return false;
    return result_->rowUpdated();
  }

  bool result::was_null(){
    if(result_ == nullptr) return false;
    return result_->wasNull();
  }

  ramrod::mysql::result& result::operator()(sql::ResultSet *result){
    close();
    result_ = result;
    return *this;
  }

  ramrod::mysql::result& result::operator=(sql::ResultSet *result){
    close();
    result_ = result;
    return *this;
  }

  ramrod::mysql::result& result::operator=(ramrod::mysql::result &result){
    close();
    result_ = (sql::ResultSet*)result;
    return *this;
  }

  ramrod::mysql::result* result::operator=(ramrod::mysql::result *result){
    close();
    result_ = (sql::ResultSet*)result;
    return this;
  }

  ramrod::mysql::result::operator sql::ResultSet*(){
    return result_;
  }

} // namespace ramrod::mysql
