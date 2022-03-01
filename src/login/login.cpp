#include "login.h"

#include <x2struct/x2struct.hpp>

#include <base64.h>
#include <ctime>
#include <database/database.h>
#include <des.h>
#include <md5.h>

bool LoginServerImp::Check() {
    std::ostringstream stream;
    stream << "select password from user_info where user_name=" << user_;
    sql::SqlRecord record = sql::DataBaseManager::GetInstance()->Query(stream.str());
    if (record.empty()) {
        LOGERROR << "get user info error:" << sql::DataBaseManager::GetInstance()->GetLastError();
        return false;
    }

    if (record.size() == 1) {
        return record.front()["passwd"] == pwd_;
    }
    return false;
}

bool LoginServerImp::SetToken() {
    std::string token = CreateToken();
    return true;
}

std::string LoginServerImp::CreateToken() {

    //产生4个1000以内的随机数
    int rand_num[4];

    //设置随机种子
    std::srand(std::time(nullptr));
    for (int i = 0; i < 4; ++i) {
        rand_num[i] = std::rand() % 1000;//随机数
    }

    std::ostringstream ostream;
    ostream << user_ << rand_num[0] << rand_num[1] << rand_num[2] << rand_num[3];

    LOGINFO << "token tmp = " << ostream.str();

    //加密
    char enc_tmp[1024 * 2];
    int enc_len = 0;
    int ret = DesEnc((unsigned char *) ostream.str().c_str(), ostream.str().size(),
                     (unsigned char *) enc_tmp, &enc_len);
    if (ret != 0) {
        LOGERROR << "DesEnc error";
        return std::string();
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
    for (int i = 0; i < 16; i++) {
        sprintf(str, "%02x", decrypt[i]);
    }
    token_ = std::string(str);
    return token_;
}

const std::string &LoginServerImp::GetToken() { return token_; }