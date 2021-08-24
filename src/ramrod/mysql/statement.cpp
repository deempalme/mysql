#include "ramrod/mysql/statement.h"

#include <cstdarg>
#include <mysql_connection.h>

#include "ramrod/mysql/connection.h"
#include "ramrod/mysql/result.h"
#include "ramrod/mysql/result_metadata.h"

namespace ramrod::mysql {
  statement::statement(ramrod::mysql::connection *connection) :
    ramrod::mysql::parameter(this),
    connection_{nullptr},
    statement_{nullptr},
    result_{new mysql::result()},
    metadata_{nullptr},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0},
    field_count_{0}
  {
    if(connection != nullptr) connection_ = (sql::Connection*)connection;
    if(connection_ != nullptr)
      statement_ = (sql::PreparedStatement*)connection_->createStatement();
  }

  statement::statement(ramrod::mysql::connection *connection, const std::string &sql) :
    ramrod::mysql::parameter(this),
    connection_{nullptr},
    statement_{nullptr},
    result_{new mysql::result()},
    metadata_{nullptr},
    affected_rows_{0},
    insert_id_{-1},
    num_rows_{0},
    field_count_{0}
  {
    if(connection != nullptr) connection_ = (sql::Connection*)connection;
    prepare(sql);
  }

  statement::~statement(){
    delete result_;
    close();
  }

  std::uint64_t statement::affected_rows(){
    return affected_rows_;
  }

  bool statement::close(){
    free_result();
    clear_parameters();
    if(statement_ == nullptr) return false;
    delete statement_;
    statement_ = nullptr;
    return true;
  }

  bool statement::execute(){
    if(statement_ == nullptr) return false;

    update_param();
    if(!statement_->execute()) return false;
    *result_ = statement_->getResultSet();
    metadata_ = &result_->get_metadata();

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = result_->num_rows();
    field_count_ = metadata_->column_count();
    return true;
  }

  bool statement::execute(const std::string &query){
    if(statement_ == nullptr) return false;

    clear_parameters();
    if(!statement_->execute(query)) return false;
    *result_ = statement_->getResultSet();
    metadata_ = &result_->get_metadata();

    affected_rows_ = statement_->getUpdateCount();
    num_rows_ = result_->num_rows();
    field_count_ = metadata_->column_count();
    return true;
  }

  ramrod::mysql::result statement::execute_query(const std::string &query){
    if(statement_ == nullptr) return ramrod::mysql::result();
    return ramrod::mysql::result(statement_->executeQuery(query));
  }

  int statement::execute_update(){
    if(statement_ == nullptr) return 0;
    update_param();
    affected_rows_ = statement_->executeUpdate();
    *result_ = statement_->getResultSet();
    metadata_ = &result_->get_metadata();

    num_rows_ = result_->num_rows();
    field_count_ = metadata_->column_count();
    return affected_rows_;
  }

  int statement::execute_update(const std::string &query){
    if(statement_ == nullptr) return 0;
    clear_parameters();
    affected_rows_ = statement_->executeUpdate(query);
    *result_ = statement_->getResultSet();
    metadata_ = &result_->get_metadata();

    num_rows_ = result_->num_rows();
    field_count_ = metadata_->column_count();
    return affected_rows_;
  }

  bool statement::fetch(){
    if(result_ == nullptr) return false;
    return result_->next();
  }

  bool statement::free_result(){
    if(result_ == nullptr) return false;
    return result_->free();
  }

  ramrod::mysql::result &statement::get_result(){
    return *result_;
  }

  std::int64_t statement::insert_id(){
    return insert_id_;
  }

  std::size_t statement::num_rows(){
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
