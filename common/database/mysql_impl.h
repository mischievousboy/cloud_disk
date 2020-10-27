#ifndef _H_MYSQL_IMPL_H_
#define _H_MYSQL_IMPL_H_

#include "database.h"

struct st_mysql;

namespace sql {
class MysqlImpl : public DataBaseManager {
public:
  bool Open() override;
  void Close() override;
  bool Exec(const std::string &sqlCmd) override;
  const std::vector<std::string> &GetResults() override;

private:
  st_mysql *sql_handle_ = nullptr;
  std::vector<std::string> result_vec_;
  unsigned long result_line_ = 0;
};
} // namespace sql

#endif //_H_MYSQL_IMPL_H_