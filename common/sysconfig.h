#ifndef _H_SYSCONFIG_H_
#define _H_SYSCONFIG_H_

#include "common_def.h"

namespace sysconfig{

class DiskSysConfig{
public:
    static DiskSysConfig* GetInstance();
    bool Init();
    const DataBaseCfg& GetMysqlCfg();
    const ServerAddress& GetRedisAddr();
private:
    DiskSysConfig(){};
    ~DiskSysConfig(){};
    
};
}
#endif //_H_SYSCONFIG_H_