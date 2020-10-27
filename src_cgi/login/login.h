#ifndef _H_LOGIN_H_
#define _H_LOGIN_H_

#include <log/mylog.h>
#include <string>

#define MODULE_NAME "login"
#define LOGINFOA LOGINFO(MODULE_NAME)
#define LOGERRORA LOGERROR(MODULE_NAME)

class Login {
public:
  bool Parse(const std::string &context);
  bool Check();
  bool SetToken();
  const std::string& GetToken();
private:
  std::string CreateToken();
  std::string user_;
  std::string pwd_;
  std::string token_;
};

#endif //_H_LOGIN_H_