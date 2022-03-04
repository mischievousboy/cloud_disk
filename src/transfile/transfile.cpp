//
// Created by huihu on 2022/3/4.
//

#include "transfile.h"
#include <fstream>

using namespace grpc;
using cloudDisk::fileServer::UploadReq;

::grpc::Status TranFileServiceImpl::Upload(::grpc::ServerContext *context, ::grpc::ServerReader<::cloudDisk::fileServer::UploadReq> *reader, ::cloudDisk::fileServer::UploadRsp *response) {
    do {
        UploadReq req;
        std::ofstream out;
        while (reader->Read(&req)) {
            if (req.has_metadata()) {
                out = GetFileStream(req);
            } else if (out.is_open()) {
                WriteFile(out, req);
            } else {
                std::cout << "filename and type not set" << std::endl;
            }
        }
        if(out.is_open())
            out.close();
    } while (false);
    return Status::OK;
}
std::ofstream TranFileServiceImpl::GetFileStream(const UploadReq &req) {
    std::string filename = req.metadata().name() + "." + req.metadata().type();
    std::ofstream of;
    of.open(filename, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    return of;
}
void TranFileServiceImpl::WriteFile(std::ofstream &out, const UploadReq &req) {
    out.write(req.file().buffer().c_str(), req.file().buffer().length());
}
