#include "redis_manager.h"

#include <hiredis/hiredis.h>
#include <iostream>
#include <string.h>

namespace redis {
namespace {
static RedisMgr *redis_mgr = nullptr;

}

RedisMgr *RedisMgr::GetInstance() {
  if (redis_mgr == nullptr)
    redis_mgr = new RedisMgr();

  return redis_mgr;
}

void RedisMgr::Release() {
  if (redis_mgr != nullptr) {
    delete redis_mgr;
    redis_mgr = nullptr;
  }
}

RedisMgr::~RedisMgr() { Close(); }

bool RedisMgr::Open(const std::string &ip, int port) {
  context_ = redisConnect(ip.c_str(), port);
  do {
    if (context_ == NULL) {
      std::cerr << "Redis conncet error:" << ip << " | " << port << std::endl;
      break;
    }

    if (context_->err) {
      std::cerr << "redis error:" << context_->errstr << std::endl;
      redisFree(context_);
      context_ = nullptr;
    }
  } while (0);

  return context_;
}

void RedisMgr::Close() {
  if (context_ != nullptr) {
    redisFree(context_);
    context_ = nullptr;
  }
}

bool RedisMgr::SetKeyValue(const std::string &key, const std::string &value,
                           unsigned int time) {
  redisReply *reply = NULL;
  bool ret = false;
  reply =
      (redisReply *)redisCommand(context_, "setex %s %u %s", key.c_str(), time, value.c_str());
  do {
    if (strncmp(reply->str, "OK", 3) != 0)
      break;
    ret = true;
  } while (0);
  freeReplyObject(reply);
  return ret;
}
} // namespace redis