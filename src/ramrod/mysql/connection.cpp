#include "ramrod/mysql/connection.h"

#include <mysql_driver.h>
#include <mysql_connection.h>

#include "ramrod/mysql/statement.h"

namespace ramrod::mysql {
  connection::connection() :
    driver_(sql::mysql::get_driver_instance()),
    connection_{nullptr},
    options_()
  {
  }

  connection::~connection(){
    close();
  }

  bool connection::close(){
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
      options_.erase(options_.find("OPT_CHARSET_NAME"));

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
      options_.erase(options_.find("password"));

    if(port >= 0)
      options_["port"] = port;
    else
      options_.erase(options_.find("port"));

    if(socket.size() > 0)
      options_["socket"] = socket;
    else
      options_.erase(options_.find("socket"));

    if(database.size() > 0)
      options_["schema"] = database;
    else
      options_.erase(options_.find("schema"));

    connection_ = driver_->connect(options_);

    return connection_ != nullptr && connection_->isValid();
  }

  ramrod::mysql::statement connection::create_statement(){
    if(connection_ == nullptr) return nullptr;
    return ramrod::mysql::statement(connection_);
  }

  sql::Connection *connection::get_connection(){
    return connection_;
  }

  ramrod::mysql::statement connection::init_statement(){
    if(connection_ == nullptr) return nullptr;
    return ramrod::mysql::statement(connection_);
  }

  bool connection::is_valid(){
    if(connection_ == nullptr) return false;
    return connection_->isValid();
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
    connection_ = driver_->connect(options_);
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
} // namespace ramrod::mysql
