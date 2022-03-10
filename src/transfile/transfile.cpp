//
// Created by huihu on 2022/3/4.
//

#include "transfile.h"
#include "base64.h"
#include "database/database.h"
#include "log/mylog.h"
#include "md5.h"
#include <fstream>

using namespace grpc;
using cloudDisk::fileServer::UploadReq;
using namespace cloudDisk::fileServer;

namespace {
    const std::string DATABASEPATH = "/data/";
    const std::string FILEURLHEAD = "/static/";
    const long CHUNK_SIZE = 1024 * 1024;
    std::string GetFileConvertName(const std::string &uid, const std::string &filename) {
        std::string tmpFileName = uid + filename;

        MD5_CTX md5;
        MD5Init(&md5);
        unsigned char decrypt[16];
        MD5Update(&md5, (unsigned char *) tmpFileName.c_str(), tmpFileName.length());
        MD5Final(&md5, decrypt);

        char base64[1024 * 3];
        base64_encode((const unsigned char *) decrypt, 16, base64);

        return base64;
    }
}// namespace

::grpc::Status TranFileServiceImpl::Upload(::grpc::ServerContext *context, ::grpc::ServerReader<::cloudDisk::fileServer::UploadReq> *reader, ::cloudDisk::fileServer::UploadRsp *response) {
    do {
        UploadReq req;
        std::ofstream out;
        std::string filename;
        while (reader->Read(&req)) {
            if (req.has_metadata()) {
                filename = GetFileConvertName(req.metadata().uid(), req.metadata().name()) + "." + req.metadata().type();
                out = GetFileStream(req.metadata().type(), filename);
                if (!StoreDB(filename, req)) {
                    //TODO 是否需要停止写入磁盘的文件
                    response->set_message("file info entry db fail");
                    response->set_code(-100);
                }
            } else if (out.is_open()) {
                WriteFile(out, req);
            } else {
                std::cout << "filename and type not set" << std::endl;
                return Status::OK;
            }
        }
        if (out.is_open())
            out.close();
    } while (false);
    return Status::OK;
}

void TranFileServiceImpl::WriteFile(std::ofstream &out, const UploadReq &req) {
    out.write(req.file().buffer().c_str(), req.file().buffer().length());
}
::grpc::Status TranFileServiceImpl::GetFileInfo(::grpc::ServerContext *context, const ::cloudDisk::fileServer::GetFileInfoReq *request, ::cloudDisk::fileServer::GetFileInfoRsp *response) {
    std::ostringstream ostream;
    ostream << "select * from user_file_info where uid = " << CREATITEM(request->uid());
    sql::DataBaseManager *db = sql::DataBaseManager::GetInstance();
    if (db) {
        auto record = db->Query(ostream.str());
        for (auto item: record) {
            auto fileinfo = response->add_fileinfo();
            fileinfo->set_url(item["file_url"]);
            fileinfo->set_filename(item["file_name"]);
        }
        response->set_code(0);
    }
    return Status::OK;
}
std::ofstream TranFileServiceImpl::GetFileStream(const string &filetype, const string &filename) {
    std::string filePath = DATABASEPATH + filetype + "/" + filename;
    std::ofstream of;
    of.open(filePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    return of;
}
bool TranFileServiceImpl::StoreDB(const string &fileConvertName, const UploadReq &req) {
    string fileUrl = FILEURLHEAD + req.metadata().type() + "/" + fileConvertName;
    sql::DataBaseManager *db = sql::DataBaseManager::GetInstance();
    std::ostringstream ostream;
    ostream << "insert into user_file_info (uid,file_name,file_url) values (" << CREATITEMP(req.metadata().uid()) << CREATITEMP(req.metadata().name()) << CREATITEM(fileUrl) << ")";
    std::cout << ostream.str() << std::endl;
    if (!(db && db->Exec(ostream.str()))) {
        LOGERROR << "file info  storedb error";
        return false;
    }
    return true;
}
::grpc::Status TranFileServiceImpl::Download(::grpc::ServerContext *context, const ::cloudDisk::fileServer::DownloadReq *request, ::grpc::ServerWriter<::cloudDisk::fileServer::DownloadRsp> *writer) {
    do {
        DownloadRsp rsp;
        std::ifstream in = GetFileStream(request->info().url());
        if (!in.is_open()) {
            LOGERROR << "can not find file:" << request->info().filename() << "|url:" << request->info().url();
            rsp.mutable_base()->set_code(-100);
            rsp.mutable_base()->set_message("can not find file");
            writer->Write(rsp);
            break;
        }
        rsp.mutable_base()->set_code(0);
        writer->Write(rsp);
        char data[CHUNK_SIZE];
        while (!in.eof()) {
            in.read(data, CHUNK_SIZE);
            rsp.mutable_context()->set_buffer(data, in.gcount());
            if (!writer->Write(rsp)) {
                break;
            }
        }
    } while (false);
    return Status::OK;
}
std::ifstream TranFileServiceImpl::GetFileStream(const std::string &fileUrl) {
    std::ifstream in;
    string filename = DATABASEPATH + fileUrl.substr(8);
    in.open(filename, std::ifstream::in | std::ifstream::binary);
    return in;
}
