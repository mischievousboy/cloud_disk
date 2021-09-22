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
#include <sysconfig.h>
#include <database/database.h>
#include <redis_manager.h>

#include "login.h"

class LoginApp : public App {
public:
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

    //读取配置文件
    sysconfig::DiskSysConfig::GetInstance()->Init();

    //初始化数据库
    sql::DataBaseManager::CreateDataBase();
    sql::DataBaseManager::GetInstance()->Init(sysconfig::DiskSysConfig::GetInstance()->GetMysqlCfg());
    if(!sql::DataBaseManager::GetInstance()->Open())
        return false;

    //连接redis
    const ServerAddress& redisAddr = sysconfig::DiskSysConfig::GetInstance()->GetRedisAddr();
    return redis::RedisMgr::GetInstance()->Open(redisAddr.ip,redisAddr.port);
  }
  int RunInternal() override {
    LOGINFO << buff_;
    int ret =-1;
    do{
      if(!login.Parse(buff_)){
        ret = -2;
        break;
      }
      
      if(!login.Check())
        break;

      if(!login.SetToken()){
        ret = -3;
        break;
      }
      ret = 0;
    }while(false);
    
    if(ret == 0){
      ReturnStatus("000",login.GetToken().c_str());
    }else if(ret == -1) {
      ReturnStatus("001","pwd error");
    }else if(ret == -2){
      ReturnStatus("002","protocol error");
    }else if(ret == -3){
      ReturnStatus("003","server error");
    }
    return ret;
  }

  Login login;
  Mylog::LogManager *log_mgr_ = nullptr;
};

int main(int argc,char** argv){
    App * app = new LoginApp;
    return app->Run();
}