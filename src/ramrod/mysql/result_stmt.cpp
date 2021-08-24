#include "ramrod/mysql/result_stmt.h"

namespace ramrod::mysql {
  result_stmt::result_stmt(ramrod::mysql::result *result){

  }

  result_stmt::~result_stmt(){
  }

  template<typename ...T>
  bool result_stmt::bind_result(T &...vars){
    if(result_ == nullptr) return false;

    return true;
  }

  bool result_stmt::clear_results(){

  }

  void result_stmt::update_results(){

  }
} // namespace ramrod::mysql
