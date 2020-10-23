#ifndef _H_MYLOG_MANAGER_H_
#define _H_MYLOG_MANAGER_H_

#include "mylog.h"

#include<log4cplus/appender.h>

namespace Mylog {

class MyLogManager : public LogManager {
private:
  log4cplus::tstring filename_;
  log4cplus::tstring module_;
  long maxFileSize_;
  int maxBackupIndex_;

  log4cplus::SharedAppenderPtr file_appender_;
  log4cplus::LogLevel file_ll_;

public:
  ~MyLogManager();
  void Init(const std::string &filename,const std::string& module, long maxFileSize,int maxBackupIndex) override ;
  void SetLogLevel(LogLevel loglevel)override;
};
} // namespace log
#endif //_H_MYLOG_MANAGER_H_