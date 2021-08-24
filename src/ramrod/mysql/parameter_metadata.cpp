#include "ramrod/mysql/parameter_metadata.h"

#include <cppconn/parameter_metadata.h>

namespace ramrod::mysql {
  parameter_metadata::parameter_metadata() : meta_{nullptr}{}

  parameter_metadata::parameter_metadata(sql::ParameterMetaData *result) : meta_{result}{}

  parameter_metadata::~parameter_metadata(){}

  std::string parameter_metadata::parameter_classname(unsigned int param){
    if(meta_ == nullptr) return std::string();
    return meta_->getParameterClassName(param);
  }

  int parameter_metadata::parameter_count(){
    if(meta_ == nullptr) return 0;
    return meta_->getParameterCount();
  }

  int parameter_metadata::parameter_mode(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->getParameterMode(param);
  }

  int parameter_metadata::parameter_type(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->getParameterType(param);
  }

  std::string parameter_metadata::parameter_typename(unsigned int param){
    if(meta_ == nullptr) return std::string();
    return meta_->getParameterTypeName(param);
  }

  int parameter_metadata::precision(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->getPrecision(param);
  }

  int parameter_metadata::scale(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->getScale(param);
  }

  int parameter_metadata::is_nullable(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->isNullable(param);
  }

  bool parameter_metadata::is_signed(unsigned int param){
    if(meta_ == nullptr) return -1;
    return meta_->isSigned(param);
  }

  parameter_metadata& parameter_metadata::operator()(sql::ParameterMetaData *metadata){
    meta_ = metadata;
    return *this;
  }

  parameter_metadata& parameter_metadata::operator=(sql::ParameterMetaData *metadata){
    meta_ = metadata;
    return *this;
  }

  ramrod::mysql::parameter_metadata::operator sql::ParameterMetaData *(){
    return meta_;
  }

} // namespace ramrod::mysql
