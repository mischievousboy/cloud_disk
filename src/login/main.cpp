/**
 * @file login.cpp
 * @author pipl (huhui@hupipl.cn)
 * @brief  登录后台的CGI程序
 * @version 0.1
 * @date 2020-10-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <app.h>
#include <database/database.h>

#include "login.h"
#include "redis_manager.h"

class LoginApp : public App {
public:
    LoginApp() : App() {}

    ~LoginApp() override {
        if (log_mgr_)
            delete log_mgr_;
    }

private:
    bool Init() override {
        //初始化日志
        module_ = MODULE_NAME;
        log_mgr_ = Mylog::CreateLogMgr(MODULE_NAME);
        if (log_mgr_) {
            log_mgr_->SetLogLevel(Mylog::Info);
        }

        AddService<LoginServerImp>("login");

        //初始化数据库
        sql::DataBaseManager::CreateDataBase();
        x2struct::JsonReader reader = json_reader_->operator[]("mysql");
        sql::DataBaseManager::GetInstance()->Init(reader);
        if (!sql::DataBaseManager::GetInstance()->Open())
            return false;

        //连接redis
        reader = json_reader_->operator[]("redis");
        RedisManager::Init(reader);
        return true;
    }
    Mylog::LogManager *log_mgr_ = nullptr;
};

int main(int argc, char **argv) {
    try {
        LoginApp app;
        app.Main(argc, argv);
        return app.Run();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
#pragma clang diagnostic pop