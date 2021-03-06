#include "sysconfig.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#include "file_until.h"
#include "common_def.h"

const std::string syscfg_path ="/conf/cfg.json";

namespace sysconfig{

typedef struct{
    ServerAddress redis_addr;
    ServerAddress webserver_addr;
    ServerAddress stroge_server_addr;
    DataBaseCfg db_cfg;
    XTOSTRUCT(M(redis_addr,webserver_addr,stroge_server_addr,db_cfg));
}SystemConfig;

namespace {
static SystemConfig system_config;
static DiskSysConfig* disk_cfg = nullptr;
} // namespace

DiskSysConfig* DiskSysConfig::GetInstance(){
    if(disk_cfg == nullptr)
        disk_cfg =new DiskSysConfig();
    return disk_cfg;
}

bool DiskSysConfig::Init() {
  std::string path = file_util::GetPwd();
  path += syscfg_path;
  int fd;
  bool sucess = false;
  do {
    //没有配置文件创建配置文件
    if (!file_util::FileExist(path)) {
      fd = open(path.c_str(), O_RDWR | O_CREAT | O_APPEND, 0777);
      if (fd > 0) {
        std::string content = x2struct::X::tojson(system_config, "", 1, '\t');
        write(fd, content.c_str(), content.size());
        break;
      }
    }

    try {
      x2struct::X::loadjson(path, system_config);
    } catch (const std::exception &e) {
      std::cout << "load cfg.json error!";
    }

    
  } while (0);

  close(fd);
  return sucess;
}

const DataBaseCfg& DiskSysConfig::GetMysqlCfg(){
  return system_config.db_cfg;
}

const ServerAddress& DiskSysConfig::GetRedisAddr(){
  return system_config.redis_addr;
}

} // namespace sysconfig