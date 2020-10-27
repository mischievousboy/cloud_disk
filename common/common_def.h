#ifndef _H_COMMON_DEF_H_
#define _H_COMMON_DEF_H_

#include <x2struct/x2struct.hpp>

typedef struct{
    std::string ip = "127.0.0.1";
    int port = -1 ;
    XTOSTRUCT(M(ip,port));
}ServerAddress;

typedef struct{
    std::string database ="cloud_disk";
    std::string user ="user";
    std::string password = "passwd";
    ServerAddress address;
    XTOSTRUCT(M(database,user,password,address));
}DataBaseCfg;

#endif //_H_COMMON_DEF_H_