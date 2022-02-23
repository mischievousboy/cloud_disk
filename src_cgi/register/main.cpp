
#include <app.h>
#include <log/mylog.h>
#include <sysconfig.h>
#include <database/database.h>

#include "register.h"

#define MODULE_NAME "register"

class RegisterApp :public App {
public:
    explicit RegisterApp(grpc::Service* service): App(service){}
protected:
	bool Init() override{
		//初始化日志
		module_ = MODULE_NAME;
		log_mgr_ = Mylog::CreateLogMgr(module_);
		if (log_mgr_) {
			log_mgr_->SetLogLevel(Mylog::Info);
		}

		//读取配置文件
		//sysconfig::DiskSysConfig::GetInstance()->Init();

		//初始化数据库
		sql::DataBaseManager::CreateDataBase();
		sql::DataBaseManager::GetInstance()->Init(sysconfig::DiskSysConfig::GetInstance()->GetMysqlCfg());
		if (!sql::DataBaseManager::GetInstance()->Open())
			return false;
        return App::Init();
	}

private:
	Mylog::LogManager* log_mgr_ = nullptr;
};

int main() {
    reg::RegisterImpl reg;
	App* app = new RegisterApp(&reg);
	return app->Run();
}