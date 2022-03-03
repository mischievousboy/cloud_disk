//
// Created by huihu on 2022/3/2.
//

#include "redis_manager.h"

#include <sw/redis++/redis++.h>

using namespace sw::redis;

namespace {
    RedisManager *mgr_ = nullptr;
    ConnectionOptions ParseOpt(x2struct::JsonReader &reader) {
        ConnectionOptions opt;
        reader.convert("host", opt.host);
        reader.convert("port", opt.port);
        if (reader.has("password"))
            reader.convert("password", opt.password);
        if (reader.has("db"))
            reader.convert("db", opt.db);
        if (reader.has("timeout")) {
            int64_t timeout;
            reader.convert("timeout", timeout);
            opt.socket_timeout = std::chrono::milliseconds(timeout);
        }
        return opt;
    }
    ConnectionPoolOptions ParsePoolOpt(x2struct::JsonReader &reader) {
        ConnectionPoolOptions opt;
        reader.convert("size", opt.size);
        int64_t timeout;
        reader.convert("wait_timeout", timeout);
        opt.wait_timeout = std::chrono::milliseconds(timeout);
        reader.convert("lifetime", timeout);
        opt.connection_lifetime = std::chrono::minutes(timeout);
        return opt;
    }
}// namespace


RedisManager *RedisManager::GetInstance() {
    if (!mgr_)
        mgr_ = new RedisManager();
    return mgr_;
}
RedisManager::RedisManager() = default;

std::shared_ptr<sw::redis::Redis> RedisManager::GetRedisImpl() {
    return redis_;
}
void RedisManager::Init(x2struct::JsonReader &reader) {
    redis_ = std::make_shared<Redis>(ParseOpt(reader), ParsePoolOpt(reader));
}
