#include "ramrod/mysql/parameter.h"

#include <istream>
#include <string>
#include <cppconn/prepared_statement.h>

#include "ramrod/mysql/statement.h"


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

  parameter::~parameter(){}

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
    if(statement_ == nullptr || param_counter_ <= 0) return;
    for(const auto &param : param_vars_)
      cast_back(param.first, param.second.type(), param.second.value());
  }

  // ::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::

  void parameter::bind_parameter(){
    if(!(param_result_ = param_counter_ != param_types_.size())){
      clear_parameters();
      // TODO: throw error
    }
  }

  void parameter::bind_parameter(bool &value){
    if(exit_param('b')) return;
    statement_->setBoolean(++param_counter_, value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::boolean, (void*)&value));
  }

  void parameter::bind_parameter(double &value){
    if(exit_param('d')) return;
    statement_->setDouble(++param_counter_, value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::double64, (void*)&value));
  }

  void parameter::bind_parameter(float &value){
    if(exit_param('f')) return;
    statement_->setDouble(++param_counter_, (double)value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::float32, (void*)&value));
  }

  void parameter::bind_parameter(std::int32_t &value){
    if(exit_param('i') && exit_param('n')) return;
    statement_->setInt(++param_counter_, value);
    if(param_types_[param_counter_] == 'i'){
      statement_->setInt64(++param_counter_, value);
      param_vars_.emplace(param_counter_, mysql::param(mysql::types::int32, (void*)&value));
    }else if(param_types_[param_counter_] == 'n'){
      statement_->setNull(++param_counter_, value);
      param_vars_.emplace(param_counter_, mysql::param(mysql::types::null, (void*)&value));
    }
  }

  void parameter::bind_parameter(std::int64_t &value){
    if(exit_param('l')) return;
    statement_->setInt64(++param_counter_, value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::int64, (void*)&value));
  }

  void parameter::bind_parameter(std::istream &blob){
    if(exit_param('B')) return;
    statement_->setBlob(++param_counter_, &blob);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::blob, (void*)&blob));
  }

  void parameter::bind_parameter(const std::string &value){
    if(exit_param('s') && exit_param('I') && exit_param('t')) return;

    if(param_types_[param_counter_] == 's'){
      statement_->setString(++param_counter_, value);
      param_vars_.emplace(param_counter_, mysql::param(mysql::types::string, (void*)&value));
    }else if(param_types_[param_counter_] == 'I'){
      statement_->setBigInt(++param_counter_, value);
      param_vars_.emplace(param_counter_, mysql::param(mysql::types::big_int, (void*)&value));
    }else if(param_types_[param_counter_] == 't'){
      statement_->setDateTime(++param_counter_, value);
      param_vars_.emplace(param_counter_, mysql::param(mysql::types::datetime, (void*)&value));
    }
  }

  void parameter::bind_parameter(std::uint32_t &value){
    if(exit_param('u')) return;
    statement_->setUInt(++param_counter_, value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::uint32, (void*)&value));
  }

  void parameter::bind_parameter(std::uint64_t &value){
    if(exit_param('U')) return;
    statement_->setUInt64(++param_counter_, value);
    param_vars_.emplace(param_counter_, mysql::param(mysql::types::uint64, (void*)&value));
  }

  void parameter::cast_back(const mysql::index index, const mysql::types type, void *value){
    switch(type){
      case ramrod::mysql::types::big_int:
        statement_->setBigInt(index, *static_cast<std::string*>(value));
      break;
      case ramrod::mysql::types::blob:
        statement_->setBlob(index, static_cast<std::istream*>(value));
      break;
      case ramrod::mysql::types::boolean:
        statement_->setBoolean(index, *static_cast<bool*>(value));
      break;
      case ramrod::mysql::types::datetime:
        statement_->setDateTime(index, *static_cast<std::string*>(value));
      break;
      case ramrod::mysql::types::double64:
        statement_->setDouble(index, *static_cast<double*>(value));
      break;
      case ramrod::mysql::types::float32:
        statement_->setDouble(index, static_cast<double>(*(float*)value));
      break;
      case ramrod::mysql::types::int32:
        statement_->setInt(index, *static_cast<std::int32_t*>(value));
      break;
      case ramrod::mysql::types::int64:
        statement_->setInt64(index, *static_cast<std::int64_t*>(value));
      break;
      case ramrod::mysql::types::uint32:
        statement_->setUInt(index, *static_cast<std::uint32_t*>(value));
      break;
      case ramrod::mysql::types::uint64:
        statement_->setUInt64(index, *static_cast<std::uint64_t*>(value));
      break;
      case ramrod::mysql::types::string:
        statement_->setString(index, *static_cast<std::string*>(value));
      break;
      case ramrod::mysql::types::null:
        statement_->setNull(index, *static_cast<int*>(value));
      break;
      default: break;
    }
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

  bool parameter::exit_param(const char type){
    if(statement_ == nullptr){
      error_param(error::no_statement);
      return true;
    }else if((param_counter_ + 1) > param_types_.size()){
      error_param(error::param_count_error);
      return true;
    }else if(param_types_[param_counter_] != type){
      error_param(error::param_type_error);
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
