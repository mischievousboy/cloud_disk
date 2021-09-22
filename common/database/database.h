#ifndef  _H_DATABASE_H_
#define  _H_DATABASE_H_

#include <string>

#include "../common_def.h"

namespace sql {

typedef enum {
  DB_Mysql,
} DB_Type;

#define DB_DEFAULT_TYPE DB_Type::DB_Mysql

class DataBaseManager {
public:
  static DataBaseManager *CreateDataBase(DB_Type type = DB_DEFAULT_TYPE);
  static DataBaseManager *GetInstance();
  static void Release();
  virtual ~DataBaseManager();
  void Init(const DataBaseCfg &db_cfg);
  virtual bool Open() = 0;
  virtual void Close(){};
  virtual bool Exec(const std::string &sqlCmd) = 0;
  virtual const std::vector<std::string> &GetResults() = 0;
  virtual const std::string& GetLastError(){
    return error_str_;
  }
protected:
  DataBaseManager(){};
  DataBaseManager(const DataBaseManager& other) = delete;
  DataBaseManager& operator=(const DataBaseManager& other) = delete;
  std::string error_str_;
  DataBaseCfg db_cfg_;
};

} // namespace sql

#endif // _H_DATABASE_H_