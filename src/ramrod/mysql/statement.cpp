#include "ramrod/mysql/statement.h"

#include <cstdarg>
#include <mysql_connection.h>

#include "ramrod/mysql/connection.h"

namespace ramrod::mysql {
  statement::statement(ramrod::mysql::connection *connection) :
    connection_{connection->get_connection()},
    statement_{nullptr},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0},
    param_result_{false},
    param_types_(),
    param_counter_{0}
  {
    if(connection_ != nullptr)
      statement_ = connection_->prepareStatement("");
  }

  statement::~statement(){
    close();
  }

  std::uint64_t statement::affected_rows(){
    return affected_rows_;
  }

  template<typename ...T>
  bool statement::bind_param(const std::string &types, T &...vars){
    if(statement_ == nullptr || types.size() <= 0) return false;

    // TODO: check if this is called more than once per call
    clear_parameters();
    param_types_ = types;

    bind_parameter(vars...);

    return param_result_;
  }

  template<typename ...T>
  bool statement::bind_result(T &...vars){
    if(statement_ == nullptr) return false;

    return true;
  }

  bool statement::close(){
    if(statement_ == nullptr) return false;
    delete statement_;
    statement_ = nullptr;
    return true;
  }

  bool statement::execute(){
    if(statement_ == nullptr) return false;
    if(!statement_->execute()) return false;

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = statement_->getMaxRows();
    return true;
  }

  bool statement::execute(const std::string &query){
    if(statement_ == nullptr) return false;
    if(!statement_->execute(query)) return false;

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = statement_->getMaxRows();
    return true;
  }

  bool statement::fetch(){
    if(statement_ == nullptr) return false;
    return statement_->getMoreResults();
  }

  bool statement::free_result(){
    if(statement_ == nullptr) return false;
  }

  std::int64_t statement::insert_id(){
    return insert_id_;
  }

  std::uint64_t statement::num_rows(){
    return num_rows_;
  }

  bool statement::prepare(const std::string &query){
    if(statement_ != nullptr) return false;
    statement_ = connection_->prepareStatement(query);
    return statement_ != nullptr;
  }

  void statement::reset(){
    close();
  }

  // ::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::

  void statement::bind_parameter(){
    param_result_ = (param_counter_ + 1) != param_types_.size();
  }

  void statement::bind_parameter(bool &value){
    if(exit_param()) return;
    statement_->setBoolean(param_counter_++, value);
  }

  void statement::bind_parameter(double &value){
    if(exit_param()) return;
    statement_->setDouble(param_counter_++, value);
  }

  void statement::bind_parameter(float &value){
    if(exit_param()) return;
    statement_->setDouble(param_counter_++, (double)value);
  }

  void statement::bind_parameter(std::int32_t &value){
    if(exit_param()) return;
    statement_->setInt(param_counter_++, value);
  }

  void statement::bind_parameter(std::int64_t &value){
    if(exit_param()) return;
    statement_->setInt64(param_counter_++, value);
  }

  void statement::bind_parameter(std::istream &blob){
    if(exit_param()) return;
    statement_->setBlob(param_counter_++, &blob);
  }

  void statement::bind_parameter(std::string &value){
    if(exit_param()) return;
    statement_->setString(param_counter_++, value);
  }

  void statement::bind_parameter(std::uint32_t &value){
    if(exit_param()) return;
    statement_->setUInt(param_counter_++, value);
  }

  void statement::bind_parameter(std::uint64_t &value){
    if(exit_param()) return;
    statement_->setUInt64(param_counter_++, value);
  }

  void statement::clear_parameters(){
    if(statement_ == nullptr) return;
    param_counter_ = 0;
    param_types_.clear();
    statement_->clearParameters();
  }

  void statement::error_param(){

  }

  bool statement::exit_param(){
    if(statement_ == nullptr) return true;
    if((param_counter_ + 1) >= param_types_.size()){
      error_param();
      return true;
    }
    return false;
  }

} // namespace ramrod::mysql
