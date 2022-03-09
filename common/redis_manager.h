//
// Created by huihu on 2022/3/2.
//

#ifndef CLOUD_DISK_REDIS_MANAGER_H
#define CLOUD_DISK_REDIS_MANAGER_H

#include <functional>
#include <iostream>

#include "log/mylog.h"
#include "x2struct/json_reader.h"

#include <sw/redis++/redis++.h>

using namespace sw::redis;

class RedisManager {
public:
    static void Init(x2struct::JsonReader &reader);
    RedisManager(const RedisManager &oth) = delete;
    RedisManager &operator=(const RedisManager &oth) = delete;
    static std::shared_ptr<sw::redis::Redis> GetRedisImpl();
    template<class RETURN, class... Args>
    static std::pair<bool, RETURN> Exec(RETURN (sw::redis::Redis::*f)(Args...), const std::string& funcName, Args &&...args) {
        RETURN ret;
        std::function<RETURN(Args & ...)> func = std::bind(f, GetRedisImpl().get(), std::forward<Args>(args)...);
        try {
            ret = func(args...);
            return std::make_pair(true, ret);
        } catch (const Error &err) {
            LOGERROR << funcName << " call error:" << err.what();
        }
        return std::make_pair(false, ret);
    }

private:
    explicit RedisManager();
    static std::shared_ptr<sw::redis::Redis> redis_;
};

#endif//CLOUD_DISK_REDIS_MANAGER_H
