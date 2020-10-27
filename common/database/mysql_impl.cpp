#include "mysql_impl.h"

#include <mysql/mysql.h>
#include <iostream>
#include <sstream>

namespace sql {

namespace {
void PrintfMysqlError(MYSQL *handle, const char *title,std::string& error) {
    std::ostringstream ostream;
    ostream << title << mysql_errno(handle) << mysql_error(handle);
    error = ostream.str();
    std::cerr << error << std::endl;
}
} // namespace

bool MysqlImpl::Open() {
  sql_handle_ = mysql_init(NULL);
  if (sql_handle_ == NULL) {
    std::cerr << "mysql init error" << std::endl;
    return false;
  }

  if (mysql_real_connect(sql_handle_, db_cfg_.address.ip.c_str(),
                         db_cfg_.user.c_str(), db_cfg_.password.c_str(),
                         db_cfg_.database.c_str(), db_cfg_.address.port, NULL,
                         0) == NULL) {
     PrintfMysqlError(sql_handle_,"mysql_real_conncet error :",error_str_);
    return false;
  }
  return true;
}

void MysqlImpl::Close() {
    mysql_close(sql_handle_);
}

bool MysqlImpl::Exec(const std::string &sqlCmd) {
  bool ret = false;
  MYSQL_RES *result = NULL;
  do {
    if (mysql_query(sql_handle_, sqlCmd.c_str()) != 0) {
      PrintfMysqlError(sql_handle_, "mysql_query error:",error_str_);
      break;
    }

    result = mysql_store_result(sql_handle_);
    if (result == NULL) {
      PrintfMysqlError(sql_handle_, "mysql_store_result error:",error_str_);
      break;
    }

    MYSQL_ROW row;
    result_line_ = 0;
    result_vec_.clear();
    result_vec_.shrink_to_fit();

    result_line_ = mysql_num_rows(result);
    ret = true;
    if (result_line_ == 0)
      break;
    else {
      if ((row = mysql_fetch_row(result)) != NULL) {
        for (int i = 0; i < result_line_; i++) {
          result_vec_.push_back(row[i]);
        }
      }
    }
  } while (0);
  if (result != NULL)
    mysql_free_result(result);
  return ret;
}

const std::vector<std::string> &MysqlImpl::GetResults() { return result_vec_; }
} // namespace sql