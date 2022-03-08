#ifndef _H_LOGIN_H_
#define _H_LOGIN_H_

#include "login.grpc.pb.h"
#include <log/mylog.h>
#include <string>

#define MODULE_NAME "login"

class LoginServerImp final : public cloudDisk::loginServer::LoginService::Service {
public:
    ::grpc::Status userLogin(::grpc::ServerContext *context, const ::cloudDisk::loginServer::LoginRequest *request, ::cloudDisk::loginServer::LoginReply *response) override;
    bool Check(const std::string &user, const std::string &pwd,std::string& uid);
    bool SetToken(const std::string &user, const std::string &token);

private:
    std::string CreateToken(const std::string &user);
};

#endif//_H_LOGIN_H_