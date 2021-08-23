#include "ramrod/mysql/statement.h"

#include <cstdarg>
#include <mysql_connection.h>

#include "ramrod/mysql/connection.h"
#include "ramrod/mysql/parameter.h"

namespace ramrod::mysql {
  statement::statement(ramrod::mysql::connection *connection) :
    ramrod::mysql::parameter(this),
    connection_{nullptr},
    statement_{nullptr},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0}
  {
    if(connection != nullptr) connection_ = (sql::Connection*)connection;
    if(connection_ != nullptr)
      statement_ = (sql::PreparedStatement*)connection_->createStatement();
  }

  statement::statement(ramrod::mysql::connection *connection, const std::string &sql) :
    ramrod::mysql::parameter(this),
    connection_{nullptr},
    statement_{nullptr},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0}
  {
    if(connection != nullptr) connection_ = (sql::Connection*)connection;
    prepare(sql);
  }

  statement::~statement(){
    close();
  }

  std::uint64_t statement::affected_rows(){
    return affected_rows_;
  }

  template<typename ...T>
  bool statement::bind_result(T &...vars){
    if(statement_ == nullptr) return false;

    return true;
  }

  bool statement::close(){
    if(statement_ == nullptr) return false;
    clear_parameters();
    delete statement_;
    statement_ = nullptr;
    return true;
  }

  bool statement::execute(){
    if(statement_ == nullptr) return false;

    update_param();
    if(!statement_->execute()) return false;

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = statement_->getMaxRows();
    return true;
  }

  bool statement::execute(const std::string &query){
    if(statement_ == nullptr) return false;

    clear_parameters();
    if(!statement_->execute(query)) return false;

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = statement_->getMaxRows();
    return true;
  }

  int statement::execute_update(){
    if(statement_ == nullptr) return 0;
    update_param();
    return affected_rows_ = statement_->executeUpdate();
  }

  int statement::execute_update(const std::string &query){
    if(statement_ == nullptr) return 0;
    clear_parameters();
    return affected_rows_ = statement_->executeUpdate(query);
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
    close();
    statement_ = connection_->prepareStatement(query);
    return statement_ != nullptr;
  }

  void statement::reset(){
    close();
  }

  ramrod::mysql::statement::operator sql::PreparedStatement*(){
    return statement_;
  }
} // namespace ramrod::mysql
