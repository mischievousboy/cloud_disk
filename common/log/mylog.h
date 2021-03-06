#ifndef _H_MYLOG_H_
#define _H_MYLOG_H_

#include <sstream>
#include <string>
#include <unistd.h>

#define LOGINFO                                                        \
  Mylog::MyLogMessage(__FILE__, __LINE__, __FUNCTION__, Mylog::Info).ostrem()
#define LOGERROR                                                      \
  Mylog::MyLogMessage(__FILE__, __LINE__, __FUNCTION__, Mylog::Error).ostrem()

#define MAX_LOG_FILE_SIZE (20 * 1024 * 1024)//20M
#define MAX_LOG_FILE_BACKUP_INDEX 3

namespace Mylog {

inline static std::string GetLogPath(const std::string& module){
    char path[1024] ={0};
    getcwd(path,1024);
    return std::string(path)+"/logs/"+module+"/log";
} 

typedef enum {
  Debug,
  Info,
  Warning,
  Error,
  Fatal,
} LogLevel;

class LogManager {
public:

  virtual ~LogManager(){};
  
  /**
   * @brief 初始化输出文件
   * 
   * @param filename  文件名
   * @param maxFileSize  最大的文件大小
   * @param maxBackupIndex 保留最近文件数量，如xxx.log,xxx1.log,xxx2.log
   */
  virtual void Init(const std::string &filename,const std::string& module,
                    long maxFileSize = MAX_LOG_FILE_SIZE,
                    int maxBackupIndex = MAX_LOG_FILE_BACKUP_INDEX) = 0;
  /**
   * @brief 设置日志的级别
   * 
   * @param loglevel 
   */
  virtual void SetLogLevel(LogLevel loglevel) {}

  /**
   * @brief 初始化控制台标准输出
   * 
   * @param name 控制台title
   */
  virtual void InitForConsole(const std::string &name) {}

public:
  static LogManager *Create();
  static void Delete(LogManager *lm);
};

class MyLogMessage {
public:
  MyLogMessage(const char *file, int line, const char *func,
               LogLevel level);
  std::ostream &ostrem() { return stream_; };
  ~MyLogMessage();
  static void SetModuleName(const std::string& module) {
      module_name_ = module;
  }
private:
  std::string file_;
  int line_;
  std::string func_;
  std::ostringstream stream_;
  LogLevel level_;
  static std::string module_name_;
};


static LogManager* CreateLogMgr(const std::string& module) {
	Mylog::MyLogMessage::SetModuleName(module);
	LogManager* log_mgr = Mylog::LogManager::Create();
	if (log_mgr) {
		log_mgr->Init(Mylog::GetLogPath(module), module);
	}
	return log_mgr;
}
} // namespace log

#endif //_H_MYLOG_H_