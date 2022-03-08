#include "register.h"

#include <database/database.h>
#include <iostream>
#include <log/mylog.h>


using namespace ::grpc;

namespace {
    const std::string UIDHEAD = "CD80";
}

namespace reg {

    std::pair<int, std::string> RegisterImpl::StoreDB(const ::cloudDisk::registerServer::RegisterRequest *request) {
        sql::DataBaseManager *db = sql::DataBaseManager::GetInstance();
        std::ostringstream ostream;
        ostream << "select* from user_info where user_name = " << CREATITEM(request->name());
        if (db->Query(ostream.str()).size() != 0)
            return std::make_pair(-2, "user exist");//用户已存在

        ostream.clear();
        ostream.str("");
        ostream << "insert into user_info (uid, user_name, nick_name, password, phone, email ) values ("
                << CREATITEMP(GetUid()) << CREATITEM(request->name()) << "," << CREATITEM(request->nick_name()) << "," << CREATITEM(request->pwd()) << ","
                << CREATITEM(request->phone()) << "," << CREATITEM(request->email()) << ");";
        LOGINFO << ostream.str();
        if (!db->Exec(ostream.str()))
            return std::make_pair(-1, "sql error");


        return std::make_pair(0, "OK");
    }

    ::grpc::Status
    RegisterImpl::userRegister(::grpc::ServerContext *context, const ::cloudDisk::registerServer::RegisterRequest *request,
                               ::cloudDisk::registerServer::RegisterReply *response) {
        auto ret = StoreDB(request);
        response->set_code(ret.first);
        response->set_message(ret.second);
        return Status::OK;
    }

    std::string RegisterImpl::GetUid() {
        static int maxID = 1;
        if (maxID == -1) {
            std::string sql = "select max(id) from user_info ";
            sql::DataBaseManager *db = sql::DataBaseManager::GetInstance();
            if (db) {
                auto res = db->Query(sql);
                if (!res.empty())
                    maxID = atoi(res.front()["max(id)"].c_str());
            }
        }
        std::string res;
        if (maxID != -1) {
            string strMaxId = std::to_string(maxID);
            maxID++;
            res.assign(10, '0');
            res.replace(0, UIDHEAD.length(), UIDHEAD);
            res.replace(10 - strMaxId.length(), strMaxId.length(), strMaxId);
        }
        return res;
    }
}// namespace reg