#include "register.h"

#include <iostream>
#include <log/mylog.h>
#include <database/database.h>

#define  MARKB "'"
#define  MARKE "' "

#define CREATITEM(value) MARKB << value.c_str() << MARKE

using namespace ::grpc;

namespace reg {

    std::pair<int, std::string> RegisterImpl::StoreDB(const ::cloudDisk::registerServer::RegisterRequest *request) {
        sql::DataBaseManager *db = sql::DataBaseManager::GetInstance();
        std::ostringstream ostream;
        ostream << "select* from user_info where user_name = " << CREATITEM(request->name());
        if (db->Exec(ostream.str())) {
            std::vector<std::string> res = db->GetResults();
            if (res.size() != 0) {
                return std::make_pair(-2, "user exist"); //用户已存在
            }
            ostream.clear();
            ostream.str("");
            ostream << "insert into user_info ( user_name, nick_name, password, phone, email ) values ("
                    <<  CREATITEM(request->name()) << "," << CREATITEM(request->nick_name()) << "," << CREATITEM(request->pwd()) << ","
                    << CREATITEM(request->phone()) << "," << CREATITEM(request->email()) << ");";
            if (!db->Exec(ostream.str()))
                return std::make_pair(-1, "sql error");
        }

        return std::make_pair(0,"OK");
    }

    ::grpc::Status
    RegisterImpl::userRegister(::grpc::ServerContext *context, const ::cloudDisk::registerServer::RegisterRequest *request,
                               ::cloudDisk::registerServer::RegisterReply *response) {
        auto ret = StoreDB(request);
        response->set_code(ret.first);
        response->set_message(ret.second);
        return Status::OK;
    }
}