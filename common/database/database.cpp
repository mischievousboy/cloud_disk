#include "database.h"

#include "mysql_impl.h"
namespace sql{

namespace{
    static DataBaseManager* db_mgr =nullptr;
}

DataBaseManager::~DataBaseManager(){
    Close();
}

DataBaseManager* DataBaseManager::CreateDataBase(DB_Type type){
    if(db_mgr !=nullptr)
        return db_mgr;
    switch(type){
        case DB_Mysql:{
            db_mgr = new MysqlImpl();
            return db_mgr;
        }
    }
}

DataBaseManager *DataBaseManager::GetInstance() {
  if (db_mgr != nullptr)
    return db_mgr;
  else {
    return CreateDataBase(DB_DEFAULT_TYPE);
  }
}

void DataBaseManager::Release(){
    if(db_mgr != nullptr){
        delete db_mgr;
        db_mgr = nullptr;
    }
}

void DataBaseManager::Init(const DataBaseCfg& db_cfg){
    db_cfg_ = db_cfg;
}

}