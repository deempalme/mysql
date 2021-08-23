#include "ramrod/mysql/param.h"

#include <istream>
#include <string>

namespace ramrod::mysql {
  param::param(const ramrod::mysql::types type, void *variable) :
    type_{type},
    variable_{variable}
  {
  }

  param::~param(){
    /*
    switch(type_){
      case ramrod::mysql::types::big_int:
        delete static_cast<std::string*>(value_);
      break;
      case ramrod::mysql::types::blob:
        delete static_cast<std::istream*>(value_);
      break;
      case ramrod::mysql::types::boolean:
        delete static_cast<bool*>(value_);
      break;
      case ramrod::mysql::types::datetime:
        delete static_cast<std::string*>(value_);
      break;
      case ramrod::mysql::types::double32:
        delete static_cast<double*>(value_);
      break;
      case ramrod::mysql::types::int32:
        delete static_cast<std::int32_t*>(value_);
      break;
      case ramrod::mysql::types::int64:
        delete static_cast<std::int64_t*>(value_);
      break;
      case ramrod::mysql::types::uint32:
        delete static_cast<std::uint32_t*>(value_);
      break;
      case ramrod::mysql::types::uint64:
        delete static_cast<std::uint64_t*>(value_);
      break;
      default: break;
    }
    */
  }
} // namespace ramrod::mysql
