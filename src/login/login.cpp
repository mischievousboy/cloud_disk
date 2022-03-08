#include "login.h"

#include <x2struct/x2struct.hpp>

#include "redis_manager.h"
#include "sw/redis++/redis++.h"
#include <base64.h>
#include <ctime>
#include <database/database.h>
#include <des.h>
#include <md5.h>

namespace {
    const std::string TOKENKEY = "HT_";
}

bool LoginServerImp::Check(const std::string &user, const std::string &pwd, std::string &uid) {
    std::ostringstream stream;
    stream << "select password,uid from user_info where user_name=" << CREATITEM(user);
    sql::SqlRecord record = sql::DataBaseManager::GetInstance()->Query(stream.str());
    if (record.empty()) {
        LOGERROR << "get user info error:" << sql::DataBaseManager::GetInstance()->GetLastError();
        return false;
    }

    if (record.size() == 1) {
        if (record.front()["password"] == pwd) {
            uid = record.front()["uid"];
            return true;
        } else
            LOGERROR << "user:" << user << " pwd not match";
    }
    return false;
}

bool LoginServerImp::SetToken(const std::string &user, const std::string &token) {
    std::shared_ptr<sw::redis::Redis> redis = RedisManager::GetInstance()->GetRedisImpl();
    if (!redis)
        return false;
    try{
         return redis->set(TOKENKEY + user, token);
    } catch (const sw::redis::Error& error){
        LOGERROR << "redis error:"  << error.what();
        return false;
    }
}

std::string LoginServerImp::CreateToken(const std::string &user) {

    //产生4个1000以内的随机数
    int rand_num[4];

    //设置随机种子
    std::srand(std::time(nullptr));
    for (int i = 0; i < 4; ++i) {
        rand_num[i] = std::rand() % 1000;//随机数
    }

    std::ostringstream ostream;
    ostream << user << rand_num[0] << rand_num[1] << rand_num[2] << rand_num[3];

    LOGINFO << "token tmp = " << ostream.str();

    //加密
    char enc_tmp[1024 * 2];
    int enc_len = 0;
    int ret = DesEnc((unsigned char *) ostream.str().c_str(), ostream.str().size(),
                     (unsigned char *) enc_tmp, &enc_len);
    if (ret != 0) {
        LOGERROR << "DesEnc error"
                 << " user:" << user;
        return {};
    }

    // to base64
    char base64[1024 * 3];
    base64_encode((const unsigned char *) enc_tmp, enc_len, base64);// base64编码
    LOGINFO << "base64 = " << base64;

    // to md5
    MD5_CTX md5;
    MD5Init(&md5);
    unsigned char decrypt[16];
    MD5Update(&md5, (unsigned char *) base64, strlen(base64));
    MD5Final(&md5, decrypt);

    char str[100] = {0};
    for (size_t i = 0; i < 16; i++) {
        sprintf(str + i, "%02x", decrypt[i]);
    }
    return str;
}

::grpc::Status LoginServerImp::userLogin(::grpc::ServerContext *context, const ::cloudDisk::loginServer::LoginRequest *request, ::cloudDisk::loginServer::LoginReply *response) {
    do {
        //        if (ret != 0) {
        //            LOGERROR << "pwd DesEnc error";
        //            response->set_code(-1);
        //            response->set_message("pwd DesEnc error");
        //            break;
        //        }
        std::string uid;
        if (!Check(request->username(), request->pwd(), uid)) {
            response->set_code(100);
            response->set_message("account pwd not match");
            break;
        }
        std::string token = CreateToken(uid);
        if (token.empty()) {
            response->set_code(-2);
            response->set_message("server error: createToken error");
            break;
        }
        if (SetToken(uid, token)) {
            response->set_code(0);
            response->set_message("ok");
            response->set_token(token);
            response->set_uid(uid);
        } else {
            response->set_code(-3);
            response->set_message("token store server error");
        }
    } while (false);
    return grpc::Status::OK;
}
