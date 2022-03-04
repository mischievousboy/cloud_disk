//
// Created by huihu on 2022/3/4.
//

#include <iostream>
#include <memory>
#include <string>

#include <fstream>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "../transfile.grpc.pb.h"

#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using namespace grpc;
using fileImp = cloudDisk::fileServer::TransFileService;
using cloudDisk::fileServer::UploadReq;
using cloudDisk::fileServer::UploadRsp;

#define CHUNK_SIZE 1024 * 1024

class TransFileClient {
public:
    TransFileClient(std::shared_ptr<Channel> channel)
        : stub_(fileImp ::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string &filename) {
        UploadReq chunk;
        char data[CHUNK_SIZE];
        UploadRsp stats;
        ClientContext context;
        std::ifstream infile;
        int len = 0;
        struct timeval start, end;

        gettimeofday(&start, NULL);
        infile.open(filename, std::ifstream::in | std::ifstream::binary);
        if (!infile.is_open())
            return "file open error";
        std::unique_ptr<ClientWriter<UploadReq>> writer(stub_->Upload(&context, &stats));
        cloudDisk::fileServer::MetaData metaData;
        chunk.mutable_metadata()->set_type("txt");
        chunk.mutable_metadata()->set_name(filename);
        writer->Write(chunk);
        while (!infile.eof()) {
            infile.read(data, CHUNK_SIZE);
            chunk.mutable_file()->set_buffer(data, infile.gcount());
            if (!writer->Write(chunk)) {
                break;
            }
            len += infile.gcount();
        }
        writer->WritesDone();
        Status status = writer->Finish();
        if (status.ok()) {
            gettimeofday(&end, NULL);
            std::cout << (end.tv_sec - start.tv_sec) + (double) (end.tv_usec - start.tv_usec) / 1000000 << std::endl;
        } else {
            std::cout << status.error_code() << std::endl;
            std::cout << "TransferFile rpc failed." << std::endl;
        }
        return stats.message();
    }

private:
    std::unique_ptr<fileImp ::Stub> stub_;
};

int main(int argc, char **argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    std::string target_str;
    std::string arg_str("--target");
    if (argc > 1) {
        std::string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != std::string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                target_str = arg_val.substr(start_pos + 1);
            } else {
                std::cout << "The only correct argument syntax is --target="
                          << std::endl;
                return 0;
            }
        } else {
            std::cout << "The only acceptable argument is --target=" << std::endl;
            return 0;
        }
    } else {
        target_str = "localhost:50051";
    }
    TransFileClient greeter(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string user("login.json");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}
