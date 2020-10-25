#ifndef _H_SYSCONFIG_H_
#define _H_SYSCONFIG_H_

#include <x2struct/x2struct.hpp>

namespace sysconfig{

typedef struct{
    std::string ip = "127.0.0.1";
    int port = -1 ;
    XTOSTRUCT(M(ip,port));
}ServerAddress;

typedef struct{
    std::string database ="clould_disk";
    std::string user ="user";
    std::string password = "passwd";
    ServerAddress address;
    XTOSTRUCT(M(database,user,password,address));
}MysqlCfg;



class DiskSysConfig{
public:
    static DiskSysConfig* GetInstance();
    bool Init();
    const MysqlCfg& GetMysqlCfg();
    const ServerAddress& GetRedisAddr();
private:
    DiskSysConfig(){};
    ~DiskSysConfig(){};
    
};
}
#endif //_H_SYSCONFIG_H_