#include "app.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

using namespace grpc;
//#include <fcgi_config.h>
//#include <fcgi_stdio.h>
//#include <stdlib.h>
//
//#include "convert.h"
//#include "log/mylog.h"
//#include <x2struct/x2struct.hpp>

//int App::Run() {
//  if (!Init())
//    return -1;
//  while (FCGI_Accept() >= 0) {
//    char *content_length = getenv("CONTENT_LENGTH");
//    int len = 0;
//
//    printf("Content-type: text/html\r\n\r\n");
//
//    if (content_length != NULL)
//      len = convert::StringToNumber<int>(content_length);
//    else
//      LOGINFO << "len = 0,No data from standard input";
//
//    if (len <= 0) {
//      printf("No data from standard input.<p>\n");
//    } else {
//      char buf[4 * 1024] = {0};
//      int ret = 0;
//      ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
//      if (ret == 0) {
//        LOGERROR << "fread(buf, 1, len, stdin) err";
//        continue;
//      }
//      if (!buff_.empty())
//          buff_.clear();
//      buff_.append(buf);
//      RunInternal();
//    }
//  }
//  return 0;
//}

//void App::ReturnStatus(const char* status,const char* msg /*= NULL*/){
//  x2struct::JsonWriter write;
//  write.convert("code",status);
//  if(msg != NULL)
//    write.convert("message",msg);
//  printf(write.toStr().c_str()); 
//}
App::App(grpc::Service* service):service_(service){

}

bool App::Init() {

    std::string server_address("0.0.0.0:50051");

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(service_);
    // Finally assemble the server.
    server_ = move(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    return true;
}

int App::Run() {
    Init();
    server_->Wait();
    return 0;
}