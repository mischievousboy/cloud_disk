#ifndef _H_MYSQL_IMPL_H_
#define _H_MYSQL_IMPL_H_

#include "database.h"

struct MYSQL;

namespace sql {
    class MysqlImpl : public DataBaseManager {
    public:
        ~MysqlImpl() override;
        bool Open() override;
        void Close() override;
        bool Exec(const std::string &sqlCmd) override;
        SqlRecord Query(const std::string &sqlCmd) override;

    private:
        MYSQL *sql_handle_ = nullptr;
    };
}// namespace sql

#endif//_H_MYSQL_IMPL_H_