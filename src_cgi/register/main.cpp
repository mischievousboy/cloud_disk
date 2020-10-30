
#include <app.h>
#include <log/mylog.h>
#include <sysconfig.h>
#include <database/database.h>

#include "register.h"

#define MODULE_NAME "register"

class RegisterApp :public App {
protected:
	bool Init() override{
		//初始化日志
		module_ = MODULE_NAME;
		log_mgr_ = Mylog::CreateLogMgr(module_);
		if (log_mgr_) {
			log_mgr_->SetLogLevel(Mylog::Info);
		}

		//读取配置文件
		sysconfig::DiskSysConfig::GetInstance()->Init();

		//初始化数据库
		sql::DataBaseManager::CreateDataBase();
		sql::DataBaseManager::GetInstance()->Init(sysconfig::DiskSysConfig::GetInstance()->GetMysqlCfg());
		if (!sql::DataBaseManager::GetInstance()->Open())
			return false;
	}

	int RunInternal()override {
		LOGINFO << buff_;
		int ret = 0;
		do {
			if (!reg_.Parse(buff_)) {
				ret = -3;
				break;
			}
			ret = reg_.StoreDB();
		} while (0);

		if (ret == 0) {
			ReturnStatus("000", "OK");
		}
		else if (ret == -1) {
			ReturnStatus("001", "register error");
		}
		else if (ret == -2) {
			ReturnStatus("002", "user exsit");
		}
		else if (ret == -3) {
			ReturnStatus("003", "protocol error");
		}
	}
private:
	Mylog::LogManager* log_mgr_ = nullptr;
	reg::MyRegister reg_;
};

int main() {
	App* app = new RegisterApp;
	return app->Run();
}