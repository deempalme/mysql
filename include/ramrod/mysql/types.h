#ifndef RAMROD_MYSQL_TYPES_H
#define RAMROD_MYSQL_TYPES_H

namespace ramrod::mysql {
  typedef unsigned int index;

  enum class types : int {
    none     = 0,
    big_int  = 1,
    blob     = 2,
    boolean  = 3,
    datetime = 4,
    double64 = 5,
    float32  = 6,
    int32    = 7,
    int64    = 8,
    uint32   = 9,
    uint64   = 10,
    null     = 11,
    string   = 12
  };

  namespace error {
    enum code : int {
      driver_not_found = 1,
      no_statement,
      param_count_error,
      param_count_match_error,
      param_type_error,
      no_result,
      result_count_match_error
    };
  }
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_TYPES_H
