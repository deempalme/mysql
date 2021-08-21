#include "ramrod/mysql/result.h"

#include <mysql_connection.h>
#include <cppconn/resultset.h>

#include "ramrod/mysql/connection.h"

namespace ramrod::mysql {
  result::result(ramrod::mysql::connection *connection) :
    connection_{connection->get_connection()},
    row_count_{0}
  {

  }

  result::~result(){
    close();
  }

  void result::after_last(){

  }

  void result::before_fisrt(){

  }

  bool result::close(){
    if(result_ == nullptr) return false;
    delete result_;
    result_ = nullptr;
    return true;
  }

  bool result::fetch(){
    if(result_ == nullptr) return false;
    return result_->next();
  }

  bool result::first(){

  }

  bool result::free(){
    // TODO: is this right?
    if(result_ == nullptr) return false;
    result_->close();
    return true;
  }

  bool result::free_result(){
    return free();
  }

  std::istream *result::get_blob(std::uint32_t index) const {
    if(result_ == nullptr) return nullptr;
    return result_->getBlob(index);
  }

  std::istream *result::get_blob(const std::string &label) const {
    if(result_ == nullptr) return nullptr;
    return result_->getBlob(label);
  }

  bool result::get_bool(std::uint32_t index) const {
    if(result_ == nullptr) return false;
    return result_->getBoolean(index);
  }

  bool result::get_bool(const std::string &label) const {
    if(result_ == nullptr) return false;
    return result_->getBoolean(label);
  }

  long double result::get_double(std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getDouble(index);
  }

  long double result::get_double(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getDouble(label);
  }

  std::int32_t result::get_int(std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getInt(index);
  }

  std::int32_t result::get_int(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getInt(label);
  }

  std::int64_t result::get_int64(std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getInt64(index);
  }

  std::int64_t result::get_int64(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getInt64(label);
  }

  std::uint32_t result::get_uint(std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt(index);
  }

  std::uint32_t result::get_uint(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt(label);
  }

  std::uint64_t result::get_uint64(std::uint32_t index) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt64(index);
  }

  std::uint64_t result::get_uint64(const std::string &label) const {
    if(result_ == nullptr) return 0;
    return result_->getUInt64(label);
  }

  std::string result::get_string(std::uint32_t index) const {
    if(result_ == nullptr) return std::string();
    return result_->getString(index);
  }

  std::string result::get_string(const std::string &label) const {
    if(result_ == nullptr) return std::string();
    return result_->getString(label);
  }

  std::size_t result::num_rows(){
    return row_count_;
  }

  std::size_t result::row_count(){
    return row_count_;
  }

} // namespace ramrod::mysql
