//
// Created by huihu on 2022/3/4.
//

#ifndef CLOUD_DISK_TRANSFILE_H
#define CLOUD_DISK_TRANSFILE_H

#include "transfile.grpc.pb.h"

#define MODULE_NAME "transfile"

class TranFileServiceImpl : public cloudDisk::fileServer::TransFileService::Service {
public:
    ::grpc::Status Upload(::grpc::ServerContext* context, ::grpc::ServerReader< ::cloudDisk::fileServer::UploadReq>* reader, ::cloudDisk::fileServer::UploadRsp* response)override;

private:
    std::ofstream GetFileStream(const cloudDisk::fileServer::UploadReq& req);
    void WriteFile(std::ofstream& out,const cloudDisk::fileServer::UploadReq& req);
};


#endif//CLOUD_DISK_TRANSFILE_H
