#include "mysql_impl.h"

#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

namespace sql {

    namespace {
        void PrintfMysqlError(MYSQL *handle, const char *title, std::string &error) {
            std::ostringstream ostream;
            ostream << title << mysql_errno(handle) << mysql_error(handle);
            error = ostream.str();
            std::cerr << error << std::endl;
        }
    }// namespace

    bool MysqlImpl::Open() {
        sql_handle_ = mysql_init(nullptr);
        if (sql_handle_ == nullptr) {
            std::cerr << "mysql init error" << std::endl;
            return false;
        }

        if (!db_conf_.charset_.empty()) {
            if (mysql_options(sql_handle_, MYSQL_SET_CHARSET_NAME, db_conf_.charset_.c_str())) {
                PrintfMysqlError(sql_handle_, "mysql_options charset error :", error_str_);
                return false;
            }
        }

        if (mysql_real_connect(sql_handle_, db_conf_.host_.c_str(),
                               db_conf_.user_.c_str(), db_conf_.passwd_.c_str(),
                               db_conf_.database_.c_str(), db_conf_.port_, nullptr,
                               db_conf_.flag_) == nullptr) {
            PrintfMysqlError(sql_handle_, "mysql_real_conncet error :", error_str_);
            return false;
        }
        return true;
    }

    void MysqlImpl::Close() {
        mysql_close(sql_handle_);
    }

    bool MysqlImpl::Exec(const std::string &sqlCmd) {
        bool ret = false;
        MYSQL_RES *result = nullptr;
        ret = (mysql_real_query(sql_handle_, sqlCmd.c_str(), sqlCmd.length()) == 0);
        if (!ret)
            PrintfMysqlError(sql_handle_, "mysql_query error:", error_str_);
        return ret;
    }


    SqlRecord MysqlImpl::Query(const std::string &sqlCmd) {
        SqlRecord data;
        MYSQL_RES *result = nullptr;
        do {
            if (!Exec(sqlCmd)) {
                PrintfMysqlError(sql_handle_, "mysql_query error:", error_str_);
                break;
            }

            result = mysql_store_result(sql_handle_);
            if (result == nullptr) {
                PrintfMysqlError(sql_handle_, "mysql_store_result error:", error_str_);
                break;
            }

            vector<string> vFields;
            MYSQL_FIELD *field = nullptr;
            while ((field = mysql_fetch_field(result))) {
                vFields.emplace_back(field->name);
            }

            MYSQL_ROW row;
            map<string, string> tmpRow;
            while ((row = mysql_fetch_row(result)) != (MYSQL_ROW) nullptr) {
                tmpRow.clear();
                unsigned long *length = mysql_fetch_lengths(result);
                for (int i = 0; i < vFields.size(); ++i) {
                    if (row[i])
                        tmpRow[vFields[i]] = string(row[i], length[i]);
                    else
                        tmpRow[vFields[i]] = "";
                }
                data.push_back(tmpRow);
            }
        } while (false);
        if (result != nullptr)
            mysql_free_result(result);
        return data;
    }
}// namespace sql