
#include <app.h>
#include <database/database.h>
#include <log/mylog.h>
#include <sysconfig.h>

#include "register.h"

#define MODULE_NAME "register"

class RegisterApp : public App {
public:
    explicit RegisterApp() {}

protected:
    bool Init() override {
        //初始化日志
        module_ = MODULE_NAME;
        log_mgr_ = Mylog::CreateLogMgr(module_);
        if (log_mgr_) {
            log_mgr_->SetLogLevel(Mylog::Info);
        }

        //读取配置文件
        //sysconfig::DiskSysConfig::GetInstance()->Init();
        AddService<reg::RegisterImpl>("register");

        //初始化数据库
        //sql::DataBaseManager::CreateDataBase();
        //sql::DataBaseManager::GetInstance()->Init(sysconfig::DiskSysConfig::GetInstance()->GetMysqlCfg());
        //if (!sql::DataBaseManager::GetInstance()->Open())
        //    return false;
        return true;
    }

private:
    Mylog::LogManager *log_mgr_ = nullptr;
};

int main(int argc, char *argv[]) {
    try {
        RegisterApp app;
        app.Main(argc, argv);
        return app.Run();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}