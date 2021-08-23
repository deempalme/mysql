#ifndef RAMROD_MYSQL_PARAM_H
#define RAMROD_MYSQL_PARAM_H

#include <cstdint>

#include "ramrod/mysql/types.h"


namespace ramrod::mysql {
  class param
  {
  public:
    param(const ramrod::mysql::types type, void *variable);
    ~param();

  private:
    ramrod::mysql::types type_;
    void *variable_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_PARAM_H
