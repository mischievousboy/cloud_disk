#ifndef _H_REGISTER_H_
#define _H_REGISTER_H_

#include "register.grpc.pb.h"


namespace reg {
class RegisterImpl : public cloudDisk::registerServer::RegisterService::Service {
    public:
        ::grpc::Status userRegister(::grpc::ServerContext* context, const ::cloudDisk::registerServer::RegisterRequest* request, ::cloudDisk::registerServer::RegisterReply* response)override;
        std::pair<int,std::string> StoreDB(const ::cloudDisk::registerServer::RegisterRequest* request);

    private:
        std::string GetUid();
    };
}

#endif //_H_REGISTER_H_
