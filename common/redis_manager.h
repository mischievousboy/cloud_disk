#ifndef _H_REDIS_MANAGER_H_
#define _H_REDIS_MANAGER_H_

#include <string>

struct redisContext;

namespace redis {
class RedisMgr {
public:
  static RedisMgr *GetInstance();
  static void Release();
  ~RedisMgr();
  bool Open(const std::string &ip, int port);
  void Close();
  bool SetKeyValue(const std::string &key, const std::string &value,
                   unsigned int time);

private:
  RedisMgr(){};
  RedisMgr(const RedisMgr &other){};
  RedisMgr &operator=(const RedisMgr &other);
  redisContext *context_ = nullptr;
};
} // namespace redis

#endif //_H_REDIS_MANAGER_H_