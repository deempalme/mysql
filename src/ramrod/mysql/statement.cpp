#include "ramrod/mysql/statement.h"

#include <cppconn/connection.h>          // for Connection
#include <cppconn/prepared_statement.h>  // for PreparedStatement
#include "ramrod/mysql/connection.h"     // for connection
#include "ramrod/mysql/result.h"         // for result

namespace sql { class ResultSet; }


namespace ramrod::mysql {
  statement::statement(ramrod::mysql::connection *connection) :
    ramrod::mysql::parameter(this),
    ramrod::mysql::result_stmt(nullptr),
    connection_{nullptr},
    statement_{nullptr},
    result_{new mysql::result()},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0}
  {
    if(connection != nullptr) connection_ = static_cast<sql::Connection*>(*connection);
  }

  statement::statement(ramrod::mysql::connection *connection, const std::string &sql) :
    ramrod::mysql::parameter(this),
    ramrod::mysql::result_stmt(nullptr),
    connection_{nullptr},
    statement_{nullptr},
    result_{new mysql::result()},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0}
  {
    if(connection != nullptr) connection_ = static_cast<sql::Connection*>(*connection);
    prepare(sql);
  }

  statement::~statement(){
    delete result_;
    close();
  }

  std::size_t statement::affected_rows(){
    return num_rows_;
  }

  void statement::after_last(){
    result_->after_last();
  }

  void statement::before_first(){
    result_->before_fisrt();
  }

  bool statement::close(){
    clear_results();
    free_result();
    clear_parameters();
    if(statement_ == nullptr) return false;
    delete statement_;
    statement_ = nullptr;
    return true;
  }

  bool statement::execute(){
    if(statement_ == nullptr) return false;

    clear_results();
    update_param();
    if(!statement_->execute()) return false;
    *result_ = statement_->getResultSet();
    mysql::parameter::update_metadata();
    mysql::result_stmt::update_metadata(static_cast<sql::ResultSet*>(*result_));

    num_rows_ = result_->num_rows();
    return true;
  }

  bool statement::execute(const std::string &query){
    if(statement_ == nullptr) return false;

    clear_results();
    clear_parameters();
    if(!statement_->execute(query)) return false;
    *result_ = statement_->getResultSet();
    mysql::parameter::update_metadata();
    mysql::result_stmt::update_metadata(static_cast<sql::ResultSet*>(*result_));

    num_rows_ = result_->num_rows();
    return true;
  }

  ramrod::mysql::result statement::execute_query(const std::string &query){
    if(statement_ == nullptr) return ramrod::mysql::result();
    return ramrod::mysql::result(statement_->executeQuery(query));
  }

  int statement::execute_update(){
    if(statement_ == nullptr) return 0;
    clear_results();
    update_param();
    affected_rows_ = statement_->executeUpdate();
    *result_ = statement_->getResultSet();
    mysql::parameter::update_metadata();
    mysql::result_stmt::update_metadata(static_cast<sql::ResultSet*>(*result_));

    num_rows_ = result_->num_rows();
    return affected_rows_;
  }

  int statement::execute_update(const std::string &query){
    if(statement_ == nullptr) return 0;
    clear_results();
    clear_parameters();
    affected_rows_ = statement_->executeUpdate(query);
    *result_ = statement_->getResultSet();
    mysql::parameter::update_metadata();
    mysql::result_stmt::update_metadata(static_cast<sql::ResultSet*>(*result_));

    num_rows_ = result_->num_rows();
    return affected_rows_;
  }

  bool statement::fetch(){
    return next();
  }

  bool statement::first(){
    if(result_->first()){
      update_results();
      return true;
    }
    return false;
  }

  bool statement::free_result(){
    clear_results();
    mysql::result_stmt::update_metadata(nullptr);
    return result_->free();
  }

  ramrod::mysql::result &statement::get_result(){
    return *result_;
  }

  std::int64_t statement::insert_id(){
    return insert_id_;
  }

  bool statement::is_after_last(){
    return result_->is_after_last();
  }

  bool statement::is_before_first(){
    return result_->is_before_first();
  }

  bool statement::is_first(){
    return result_->is_first();
  }

  bool statement::is_last(){
    return result_->is_last();
  }

  bool statement::last(){
    if(result_->last()){
      update_results();
      return true;
    }
    return false;
  }

  bool statement::next(){
    if(result_->next()){
      update_results();
      return true;
    }
    return false;
  }

  std::size_t statement::num_rows(){
    return num_rows_;
  }

  bool statement::prepare(const std::string &query){
    close();
    statement_ = connection_->prepareStatement(query);
    mysql::parameter::update_statement(statement_);
    mysql::result_stmt::update_metadata(nullptr);
    return statement_ != nullptr;
  }

  bool statement::previous(){
    if(result_->previous()){
      update_results();
      return true;
    }
    return false;
  }

  void statement::reset(){
    close();
  }

  ramrod::mysql::statement::operator sql::PreparedStatement*(){
    return statement_;
  }
} // namespace ramrod::mysql
