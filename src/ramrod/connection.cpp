#include "ramrod/connection.h"

#include <mysql_driver.h>
#include <mysql_connection.h>

namespace ramrod {
  connection::connection() :
    driver_(sql::mysql::get_driver_instance()),
    connection_{nullptr}
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

  bool connection::connect(const std::string &hostname, const std::string &username,
                           const std::string &password, const std::string &database,
                           const int port, const std::string &socket){
    if(driver_ == nullptr) return false;

    return connection_ != nullptr;
  }
} // namespace ramrod
