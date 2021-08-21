#include "ramrod/connection.h"

#include <mysql_driver.h>
#include <mysql_connection.h>

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

  bool connection::connect(const std::string &hostname, const std::string &username,
                           const std::string &password, const std::string &database,
                           const int port, const std::string &socket){
    if(driver_ == nullptr) return false;
    if(connection_ != nullptr) close();

    options_.clear();
    options_["hostName"] = hostname;
    options_["userName"] = username;
    if(password.size() > 0)
      options_["password"] = password;
    if(port >= 0)
      options_["port"] = port;
    if(socket.size() > 0)
      options_["socket"] = socket;
    if(database.size() > 0)
      options_["schema"] = database;

    connection_ = driver_->connect(options_);

    return connection_ != nullptr && connection_->isValid();
  }

  bool connection::reconnect(){
    if(connection_ == nullptr) return false;
    connection_->reconnect();
    return connection_->isValid();
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

  bool connection::set_charset(const std::string &charset){
    if(connection_ == nullptr) return false;
    // TODO: how to do it?
    return true;
  }
} // namespace ramrod
