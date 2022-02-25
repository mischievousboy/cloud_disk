#ifndef _H_APP_H_
#define _H_APP_H_

#include <memory>
#include <string>

class Option;

struct ServerConfig {
    std::string host;
    std::string port;
};

namespace x2struct {
    class JsonReader;
}
namespace grpc {
    class Server;
    class Service;
}// namespace grpc


using ServicePtr = std::shared_ptr<grpc::Service>;


class App {
public:
    App();
    virtual ~App() = default;
    void Main(int argc, char *argv[]);
    int Run();
    virtual bool Init() = 0;

    template<class T>
    void AddService(const std::string &id) {
        service_ = std::make_pair(id, std::make_shared<T>());
    }

protected:
    void Main(const Option &opt);
    void ParseConfig(const std::string &configPath);
    void InitServer();

    std::string module_;
    std::unique_ptr<x2struct::JsonReader> json_reader_;
    //ServicePtr service_;
    std::pair<std::string,ServicePtr> service_;
    std::shared_ptr<grpc::Server> server_;
};


#endif//_H_APP_H_
