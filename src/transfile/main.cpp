#include <iostream>

#include "app.h"
#include "transfile.h"
#include "log/mylog.h"
#include "database/database.h"
#include "redis_manager.h"

class TransFileApp : public App {
public:
    TransFileApp() : App() {}

    ~TransFileApp() override {
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

        AddService<TranFileServiceImpl>("transfile");

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


int main(int argc, char *argv[]) {
    try {
        TransFileApp app;
        app.Main(argc, argv);
        return app.Run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}