//
// Created by huihu on 2022/3/2.
//

#ifndef CLOUD_DISK_REDIS_MANAGER_H
#define CLOUD_DISK_REDIS_MANAGER_H

#include "x2struct/json_reader.h"

namespace sw{
    namespace redis{
        class Redis;
    }
}

class RedisManager {
public:
    static RedisManager* GetInstance();
    void Init(x2struct::JsonReader& reader);
    RedisManager(const RedisManager& oth)=delete;
    RedisManager& operator=(const RedisManager& oth)=delete;
    std::shared_ptr<sw::redis::Redis> GetRedisImpl();
private:
    explicit RedisManager();
    std::shared_ptr<sw::redis::Redis> redis_;
};


#endif//CLOUD_DISK_REDIS_MANAGER_H
