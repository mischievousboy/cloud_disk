//
// Created by huihu on 2022/3/4.
//

#ifndef CLOUD_DISK_TRANSFILE_H
#define CLOUD_DISK_TRANSFILE_H

#include "transfile.grpc.pb.h"

#define MODULE_NAME "transfile"

class TranFileServiceImpl : public cloudDisk::fileServer::TransFileService::Service {
public:
    ::grpc::Status Upload(::grpc::ServerContext *context, ::grpc::ServerReader<::cloudDisk::fileServer::UploadReq> *reader, ::cloudDisk::fileServer::UploadRsp *response) override;
    ::grpc::Status GetFileInfo(::grpc::ServerContext *context, const ::cloudDisk::fileServer::GetFileInfoReq *request, ::cloudDisk::fileServer::GetFileInfoRsp *response) override;
    ::grpc::Status Download(::grpc::ServerContext* context, const ::cloudDisk::fileServer::DownloadReq* request, ::grpc::ServerWriter< ::cloudDisk::fileServer::DownloadRsp>* writer)override;
private:
    std::ofstream GetFileStream(const std::string &filetype, const std::string &filename);
    std::ifstream GetFileStream(const std::string &fileUrl);
    void WriteFile(std::ofstream &out, const cloudDisk::fileServer::UploadReq &req);
    bool StoreDB(const std::string &fileConvertName, const cloudDisk::fileServer::UploadReq &req);
};


#endif//CLOUD_DISK_TRANSFILE_H
