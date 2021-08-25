#include "ramrod/mysql/connection.h"

#include <cppconn/sqlstring.h>       // for SQLString
#include <mysql_driver.h>            // for get_driver_instance, MySQL_Driver
#include <mysql_connection.h>        // for escapeString
#include <map>                       // for map<>::mapped_type
#include "ramrod/mysql/result.h"     // for result
#include "ramrod/mysql/statement.h"  // for statement


namespace ramrod::mysql {
  connection::connection() :
    driver_(sql::mysql::get_driver_instance()),
    connection_{nullptr},
    options_(),
    statement_{nullptr}
  {
    // TODO: set error if driver is not found
    //if(driver_ == nullptr)
  }

  connection::connection(const std::string &hostname, const std::string &username,
                         const std::string &password, const std::string &database,
                         const int port, const std::string &socket) :
    driver_(sql::mysql::get_driver_instance()),
    connection_{nullptr},
    options_()
  {
    connect(hostname, username, password, database, port, socket);
  }

  connection::~connection(){
    close();
  }

  bool connection::close(){
    if(statement_ != nullptr){
      delete statement_;
      statement_ = nullptr;
    }
    if(connection_ == nullptr) return false;
    delete connection_;
    connection_ = nullptr;
    return true;
  }

  bool connection::closed(){
    if(connection_ == nullptr) return true;
    return connection_->isClosed();
  }

  std::string connection::charset(){
    if(connection_ == nullptr) return std::string();
    return connection_->getClientOption("OPT_CHARSET_NAME").asStdString();
  }

  bool connection::charset(const std::string &charset, const bool reconnect){
    if(charset.size() > 0)
      options_["OPT_CHARSET_NAME"] = charset;
    else
      options_.erase("OPT_CHARSET_NAME");

    if(reconnect && connection_ != nullptr)
      return this->reconnect();

    return connection_ != nullptr;
  }

  bool connection::connect(const std::string &hostname, const std::string &username,
                           const std::string &password, const std::string &database,
                           const int port, const std::string &socket){
    if(driver_ == nullptr) return false;
    if(connection_ != nullptr) close();

    options_["hostName"] = hostname;
    options_["userName"] = username;

    if(password.size() > 0)
      options_["password"] = password;
    else
      options_.erase("password");

    if(port >= 0)
      options_["port"] = port;
    else
      options_.erase("port");

    if(socket.size() > 0)
      options_["socket"] = socket;
    else
      options_.erase("socket");

    if(database.size() > 0)
      options_["schema"] = database;
    else
      options_.erase("schema");

    connection_ = dynamic_cast<sql::mysql::MySQL_Connection*>(driver_->connect(options_));

    return connection_ != nullptr && connection_->isValid();
  }

  ramrod::mysql::statement connection::create_statement(){
    if(connection_ == nullptr) return nullptr;
    return ramrod::mysql::statement(this);
  }

  std::string connection::escape_string(const std::string &string){
    if(connection_ == nullptr) return std::string();
    return connection_->escapeString(string);
  }

  ramrod::mysql::statement connection::init_statement(){
    return create_statement();
  }

  bool connection::is_valid(){
    if(connection_ == nullptr) return false;
    return connection_->isValid();
  }

  ramrod::mysql::statement connection::prepare(const std::string &sql){
    return ramrod::mysql::statement(this, sql);
  }

  ramrod::mysql::result connection::query(const std::string &query){
    if(connection_ == nullptr) return ramrod::mysql::result();
    if(statement_ != nullptr) delete statement_;
    statement_ = new ramrod::mysql::statement(this);
    return statement_->execute_query(query);
  }

  bool connection::read_only(){
    if(connection_ == nullptr) return true;
    return connection_->isReadOnly();
  }

  bool connection::read_only(const bool set_read_only){
    if(connection_ == nullptr) return false;
    connection_->setReadOnly(set_read_only);
    return true;
  }

  bool connection::reconnect(){
    if(connection_ == nullptr) return false;
    close();
    connection_ = dynamic_cast<sql::mysql::MySQL_Connection*>(driver_->connect(options_));
    return connection_ != nullptr && connection_->isValid();
  }

  std::string connection::schema(){
    if(connection_ == nullptr) return std::string();
    return connection_->getSchema().asStdString();
  }

  bool connection::schema(const std::string &schema){
    if(connection_ == nullptr) return false;
    connection_->setSchema(schema);
    return true;
  }

  bool connection::select_db(const std::string &database){
    return schema(database);
  }

  ramrod::mysql::connection::operator sql::Connection*(){
    return connection_;
  }
} // namespace ramrod::mysql
