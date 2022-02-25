#include "app.h"

#include <iostream>

#include "option.h"
#include "x2struct/json_reader.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/server_builder.h>

using namespace grpc;


int App::Run() {
    server_->Wait();
    return 0;
}
void App::Main(int argc, char **argv) {
    Option opt;
    opt.Decode(argc, argv);
    Main(opt);
}
App::App():service_() {
}
void App::Main(const Option &opt) {
    auto config = opt.getParam("config");
    if (config.empty()) {
        std::cerr << "not give config";
        exit(1);
    }

    ParseConfig(config);
    Init();
    InitServer();
}
void App::InitServer() {
    x2struct::JsonReader reader = json_reader_->operator[]("server");
    string host = "0.0.0.0";
    string port = "8888";
    if (reader.has("ip"))
        reader.convert("ip", host);
    if (reader.has("port"))
        reader.convert("port", port);
    std::string server_address = host + ":" + port;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    if (service_.second) {
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(service_.second.get());
        server_ = move(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
    } else
        std::cerr << "service do not init";
}
void App::ParseConfig(const string &configPath) {
    json_reader_.reset(new x2struct::JsonReader(configPath, true));
}
