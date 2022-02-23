#ifndef _H_APP_H_
#define _H_APP_H_

#include <string>
#include <grpcpp/grpcpp.h>

//class App {
//public:
//  int Run();
//  virtual ~App(){};
//  void ReturnStatus(const char* status,const char* msg = NULL);
//protected:
//  std::string buff_;
//  std::string module_;
//
//private:
//  virtual bool Init(){ return true; };
//  virtual int RunInternal() = 0;
//};
//#endif //_H_APP_H_

class App {
public:
    App(grpc::Service* service);
    int Run();
    virtual bool Init();
    virtual ~App()= default;

protected:
    std::unique_ptr<grpc::Server> server_;
    grpc::Service* service_;
    std::string module_;
};


#endif //_H_APP_H_
