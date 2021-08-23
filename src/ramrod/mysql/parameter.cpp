#include "ramrod/mysql/parameter.h"

#include <istream>
#include <string>
#include <cppconn/prepared_statement.h>

#include "ramrod/mysql/statement.h"

#include "ramrod/mysql/param.h"

namespace ramrod::mysql {
  parameter::parameter(ramrod::mysql::statement *statement) :
    statement_{nullptr},
    param_result_{false},
    param_in_{false},
    param_types_(),
    param_counter_{0},
    param_vars_()
  {
    if(statement != nullptr)
      statement_ = (sql::PreparedStatement*)statement;
  }

  template<typename ...T>
  bool parameter::bind_param(const std::string &types, T &...vars){
    if(statement_ == nullptr || types.size() <= 0) return false;

    // TODO: check if this is called more than once per call
    clear_parameters();
    param_types_ = types;

    // TODO: see std::forward
    bind_parameter(vars...);

    return param_result_;
  }

  void parameter::clear_parameters(){
    if((param_counter_ > 0 || param_in_) && statement_ != nullptr)
      statement_->clearParameters();
    param_result_ = false;
    param_in_ = false;
    param_types_.clear();
    param_counter_ = 0;
    param_vars_.clear();
  }

  parameter &parameter::set_big_int(const unsigned int index, const std::string &value){
    if(param_set(index))
      statement_->setBigInt(index, value);
    return *this;
  }

  parameter &parameter::set_datetime(const unsigned int index, const std::string &value){
    if(param_set(index))
      statement_->setDateTime(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const bool value){
    if(param_set(index))
      statement_->setBoolean(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const double value){
    if(param_set(index))
      statement_->setDouble(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const float value){
    if(param_set(index))
      statement_->setDouble((double)index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const std::int32_t value){
    if(param_set(index))
      statement_->setInt(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const std::int64_t value){
    if(param_set(index))
      statement_->setInt64(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, std::istream *blob){
    if(param_set(index))
      statement_->setBlob(index, blob);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const std::string &value){
    if(param_set(index))
      statement_->setString(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const std::uint32_t value){
    if(param_set(index))
      statement_->setUInt(index, value);
    return *this;
  }

  parameter &parameter::set_param(const unsigned int index, const std::uint64_t value){
    if(param_set(index))
      statement_->setUInt64(index, value);
    return *this;
  }

  parameter &parameter::set_null(const unsigned int index, const int sql_type){
    if(param_set(index))
      statement_->setNull(index, sql_type);
    return *this;
  }

  // :::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::

  void parameter::update_param(){
    if(param_counter_ <= 0) return;
  }

  // ::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::

  void parameter::bind_parameter(){
    param_result_ = param_counter_ != param_types_.size();
  }

  void parameter::bind_parameter(bool &value){
    if(exit_param()) return;
    statement_->setBoolean(++param_counter_, value);
  }

  void parameter::bind_parameter(double &value){
    if(exit_param()) return;
    statement_->setDouble(++param_counter_, value);
  }

  void parameter::bind_parameter(float &value){
    if(exit_param()) return;
    statement_->setDouble(++param_counter_, (double)value);
  }

  void parameter::bind_parameter(std::int32_t &value){
    if(exit_param()) return;
    statement_->setInt(++param_counter_, value);
  }

  void parameter::bind_parameter(std::int64_t &value){
    if(exit_param()) return;
    statement_->setInt64(++param_counter_, value);
  }

  void parameter::bind_parameter(std::istream &blob){
    if(exit_param()) return;
    statement_->setBlob(++param_counter_, &blob);
  }

  void parameter::bind_parameter(const std::string &value){
    if(exit_param()) return;
    statement_->setString(++param_counter_, value);
  }

  void parameter::bind_parameter(std::uint32_t &value){
    if(exit_param()) return;
    statement_->setUInt(++param_counter_, value);
  }

  void parameter::bind_parameter(std::uint64_t &value){
    if(exit_param()) return;
    statement_->setUInt64(++param_counter_, value);
  }

  void parameter::error_param(const mysql::error::code code){
    switch(code){
      case error::code::no_statement:
        // The statement is no open
      break;
      case error::code::param_count_error:
        // Selected a different number of variables than the defined in types
        // The number of variables is not equal to the number of types
      break;
      case error::code::param_type_error:
        // Selected a different type for a variable
      break;
      default: break;
    }
  }

  bool parameter::exit_param(){
    if((param_counter_ + 1) > param_types_.size()){
      error_param(error::no_statement);
      return true;
    }
    return false;
  }

  bool parameter::param_set(const unsigned int index){
    if(statement_ == nullptr || index == 0) return false;
    if(index <= param_types_.size()){
      param_types_[index - 1] = ' ';
      param_vars_.erase(index);
    }
    param_in_ = true;
    return true;
  }
} // namespace ramrod::mysql
