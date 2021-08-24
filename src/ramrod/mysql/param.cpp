#include "ramrod/mysql/param.h"


namespace ramrod::mysql {
  param::param(const ramrod::mysql::types type, void *variable) :
    type_{type},
    variable_{variable}
  {
  }

  ramrod::mysql::types param::type() const{
    return type_;
  }

  void *param::value() const{
    return variable_;
  }
} // namespace ramrod::mysql
