#ifndef _H_DATABASE_H_
#define _H_DATABASE_H_

#include <string>

#include "../common_def.h"

namespace sql {

    typedef enum {
        DB_Mysql,
    } DB_Type;

#define DB_DEFAULT_TYPE DB_Type::DB_Mysql

    /*!
     * 数据库配置
     */
    struct DBConf {
        std::string host_;
        std::string user_;
        std::string passwd_;
        std::string database_;
        std::string charset_;
        int port_;
        int flag_;

        DBConf() : port_(3306) {}

        void LoadFromMap(const std::map<std::string, std::string> &mapParam) {
            std::map<std::string, std::string> tmpMap = mapParam;
            host_ = tmpMap["host"];
            user_ = tmpMap["user"];
            passwd_ = tmpMap["passwd"];
            database_ = tmpMap["database"];
            charset_ = tmpMap["charset"];
            port_ = atoi(tmpMap["port"].c_str());
            flag_ = atoi(tmpMap["flag"].c_str());
        }

        void LoadFromJson(x2struct::JsonReader& reader){
            reader.convert("host",host_);
            reader.convert("user",user_);
            reader.convert("passwd",passwd_);
            reader.convert("database",database_);
            reader.convert("port",port_);
            reader.convert("flag",flag_);
        }
    };

    using SqlRecord = std::vector<std::map<std::string, std::string>>;

    class DataBaseManager {
    public:
        static DataBaseManager *CreateDataBase(DB_Type type = DB_DEFAULT_TYPE);
        static DataBaseManager *GetInstance();
        static void Release();
        virtual ~DataBaseManager();
        void Init(x2struct::JsonReader &reader);
        virtual bool Open() = 0;
        virtual void Close(){};
        /*!
         * 执行sql语句不查看返回结果集
         * @param sqlCmd
         * @return
         */
        virtual bool Exec(const std::string &sqlCmd) = 0;
        /*!
         * 查询sql执行结果集
         * @param sqlCmd
         * @return
         */
        virtual SqlRecord Query(const std::string &sqlCmd) = 0;
        /*!
         * 返回的结果集
         * @return
         */
        virtual const std::string &GetLastError() {
            return error_str_;
        }

    protected:
        DataBaseManager() = default;
        ;
        DataBaseManager(const DataBaseManager &other) = delete;
        DataBaseManager &operator=(const DataBaseManager &other) = delete;

        std::string error_str_;
        DBConf db_conf_;
    };

}// namespace sql

#endif// _H_DATABASE_H_