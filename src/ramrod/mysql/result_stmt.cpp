#include "ramrod/mysql/result_stmt.h"

#include <cppconn/resultset.h>

#include "ramrod/mysql/result_metadata.h"

namespace ramrod::mysql {
  result_stmt::result_stmt(ramrod::mysql::result *result) :
    result_{nullptr},
    metadata_{new mysql::result_metadata()},
    result_error_{false},
    result_counter_{0},
    result_total_{0},
    result_vars_()
  {
    if(result != nullptr)
      result_ = (sql::ResultSet*)result;
  }

  result_stmt::~result_stmt(){
    delete metadata_;
  }

  template<typename ...T>
  bool result_stmt::bind_result(T &...variables){
    if(result_ == nullptr) return false;

    clear_results();

    bind_results(variables...);

    return !result_error_;
  }

  void result_stmt::clear_results(){
    result_error_ = false;
    result_counter_ = 0;
    result_vars_.clear();
  }

  unsigned int result_stmt::field_count(){
    return result_total_;
  }

  // :::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::

  void result_stmt::update_results(){
    if(result_ == nullptr || result_counter_ <= 0) return;
    for(const auto &result : result_vars_)
      cast_back(result.first, result.second.type(), result.second.value());
  }

  void result_stmt::update_metadata(sql::ResultSet *new_result){
    result_ = new_result;
    if(result_ == nullptr)
      *metadata_ = (sql::ResultSetMetaData*)nullptr;
    else
      *metadata_ = result_->getMetaData();
    result_total_ = metadata_->column_count();
  }

  // ::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::

  void result_stmt::bind_results(){
    if(result_total_ != result_counter_){
      error_result(mysql::error::result_count_match_error);
      result_error_ = true;
    }
  }

  void result_stmt::bind_results(bool &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::boolean, (void*)&value));
  }

  void result_stmt::bind_results(long double &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::double64, (void*)&value));
  }

  void result_stmt::bind_results(float &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::float32, (void*)&value));
  }

  void result_stmt::bind_results(std::int32_t &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::int32, (void*)&value));
  }

  void result_stmt::bind_results(std::int64_t &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::int64, (void*)&value));
  }

  void result_stmt::bind_results(std::istream &blob){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::blob, (void*)&blob));
  }

  void result_stmt::bind_results(const std::string &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::string, (void*)&value));
  }

  void result_stmt::bind_results(std::uint32_t &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::uint32, (void*)&value));
  }

  void result_stmt::bind_results(std::uint64_t &value){
    if(exit_result()) return;
    result_vars_.emplace(++result_counter_, mysql::param(mysql::types::uint64, (void*)&value));
  }

  void result_stmt::cast_back(const mysql::index index, const mysql::types type, void *value){
    switch(type){
      case ramrod::mysql::types::blob:
        value = static_cast<void*>(result_->getBlob(index));
      break;
      case ramrod::mysql::types::boolean:
        *static_cast<bool*>(value) = result_->getBoolean(index);
      break;
      case ramrod::mysql::types::double64:
        *static_cast<long double*>(value) = result_->getDouble(index);
      break;
      case ramrod::mysql::types::float32:
        *static_cast<float*>(value) = (float)result_->getDouble(index);
      break;
      case ramrod::mysql::types::int32:
        *static_cast<std::int32_t*>(value) = result_->getInt(index);
      break;
      case ramrod::mysql::types::int64:
        *static_cast<std::int64_t*>(value) = result_->getInt64(index);
      break;
      case ramrod::mysql::types::uint32:
        *static_cast<std::uint32_t*>(value) = result_->getUInt(index);
      break;
      case ramrod::mysql::types::uint64:
        *static_cast<std::uint64_t*>(value) = result_->getUInt64(index);
      break;
      case ramrod::mysql::types::string:
        *static_cast<std::string*>(value) = result_->getString(index);
      break;
      default: break;
    }
  }

  void result_stmt::error_result(const error::code code){
    // TODO: throw error
    switch(code){
      case error::code::no_result:
        std::perror("MySQL Error: The result is no open");
      break;
      case error::code::result_count_match_error:
        std::perror("MySQL Error: the number of result bindings does not match "
                    "the number of columns in the obtained row");
      break;
      default: break;
    }
  }

  bool result_stmt::exit_result(){
    if(result_ == nullptr){
      error_result(error::no_result);
      return true;
    }else if((result_counter_ + 1) > result_total_){
      error_result(error::param_count_match_error);
      return true;
    }
    return false;
  }
} // namespace ramrod::mysql
