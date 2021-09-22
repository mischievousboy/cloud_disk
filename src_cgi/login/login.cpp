#include "login.h"

#include <x2struct/x2struct.hpp>

#include <database/database.h>
#include <des.h>
#include <base64.h>
#include <md5.h>
#include <redis_manager.h>
#include <ctime>

bool Login::Parse(const std::string &context) {
  try {
    x2struct::JsonReader reader(context);
    reader.convert("user", user_);
    reader.convert("pwd", pwd_);
  } catch (const std::exception &e) {
    LOGERROR << "login request parse error";
    return false;
  }

  return true;
}

bool Login::Check(){
    std::ostringstream stream;
    stream << "select password from user_info where user_name=" << user_;
    if(!sql::DataBaseManager::GetInstance()->Exec(stream.str())){
      LOGERROR << "get user info error:" << sql::DataBaseManager::GetInstance()->GetLastError();
      return false;
    }

    const std::vector<std::string>& results= sql::DataBaseManager::GetInstance()->GetResults();
    if(results.size() == 1){
      return results.front() == pwd_;
    }
    return false;
}

bool Login::SetToken() {
  std::string token = CreateToken();
  return redis::RedisMgr::GetInstance()->SetKeyValue(user_, token, 86400);
}

std::string Login::CreateToken() {

  //产生4个1000以内的随机数
  int rand_num[4];

  //设置随机种子
  std::srand(std::time(nullptr));
  for (int i = 0; i < 4; ++i) {
    rand_num[i] = std::rand() % 1000; //随机数
  }

  std::ostringstream ostream;
  ostream << user_ << rand_num[0] << rand_num[1] << rand_num[2] << rand_num[3];

  LOGINFO << "token tmp = " << ostream.str();

  //加密
  char enc_tmp[1024 * 2];
  int enc_len = 0;
  int ret = DesEnc((unsigned char *)ostream.str().c_str(), ostream.str().size(),
                   (unsigned char *)enc_tmp, &enc_len);
  if (ret != 0) {
    LOGERROR << "DesEnc error";
    return std::string();
  }

  // to base64
  char base64[1024 * 3];
  base64_encode((const unsigned char *)enc_tmp, enc_len, base64); // base64编码
  LOGINFO << "base64 = " << base64;

  // to md5
  MD5_CTX md5;
  MD5Init(&md5);
  unsigned char decrypt[16];
  MD5Update(&md5, (unsigned char *)base64, strlen(base64));
  MD5Final(&md5, decrypt);

  char str[100] = {0};
  for (int i = 0; i < 16; i++) {
    sprintf(str, "%02x", decrypt[i]);
  }
  token_ = std::string(str);
  return token_;
}

const std::string &Login::GetToken() { return token_; }