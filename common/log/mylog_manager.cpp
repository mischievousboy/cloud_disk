#include "mylog_manager.h"

#include <log4cplus/fileappender.h>
#include <log4cplus/logger.h>
#include <log4cplus/spi/loggingevent.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_INSTANCE_NAME "mylog"

namespace Mylog {

class MultiProcessFileTTCCLayout : public log4cplus::TTCCLayout {
public:
  MultiProcessFileTTCCLayout() : log4cplus::TTCCLayout(false) {
    process_id_ = getgid();
    if (dateFormat.empty()) {
      dateFormat = LOG4CPLUS_TEXT("%m-%d-%y %H:%M:%S,%q");
    }
  }

  virtual void
  formatAndAppend(log4cplus::tostream &output,
                  const log4cplus::spi::InternalLoggingEvent &event) override {
    if (event.getNDC().empty()) {
      output << log4cplus::helpers::getFormattedTime(
                    dateFormat, event.getTimestamp(), use_gmtime)
             << LOG4CPLUS_TEXT(" [") << process_id_ << "." << event.getThread()
             << LOG4CPLUS_TEXT("] ") << llmCache.toString(event.getLogLevel())
             << LOG4CPLUS_TEXT(" ") << event.getLoggerName()
             << LOG4CPLUS_TEXT(" ") << event.getMessage()
             << LOG4CPLUS_TEXT("\n");
    } else {
      output << log4cplus::helpers::getFormattedTime(
                    dateFormat, event.getTimestamp(), use_gmtime)
             << LOG4CPLUS_TEXT(" [") << process_id_ << "." << event.getThread()
             << LOG4CPLUS_TEXT("] ") << llmCache.toString(event.getLogLevel())
             << LOG4CPLUS_TEXT(" ") << event.getLoggerName()
             << LOG4CPLUS_TEXT(" <") << event.getNDC() << LOG4CPLUS_TEXT("> - ")
             << event.getMessage() << LOG4CPLUS_TEXT("\n");
    }
  }

private:
  pid_t process_id_;
};

void MyLogManager::Init(const std::string &filename, const std::string &module,
                        long maxFileSize, int maxBackupIndex) {
  filename_ = filename;
  maxFileSize_ = maxFileSize;
  maxBackupIndex_ = maxBackupIndex;
  module_ = module;

  log4cplus::Logger logger = log4cplus::Logger::getInstance(module_);
  log4cplus::RollingFileAppender *rfa = new log4cplus::RollingFileAppender(
      filename_, maxFileSize_, maxBackupIndex_, true ,true/*立即更新到文件*/);
  std::locale lc("zh_CN");
  rfa->imbue(lc); // 支持输出中文到文件

  file_appender_ = rfa;
  file_appender_->setName("mylog");
  file_appender_->setLayout(
      std::auto_ptr<log4cplus::Layout>(new MultiProcessFileTTCCLayout()));

  logger.addAppender(file_appender_);
  logger.setLogLevel(file_ll_);
}

void MyLogManager::SetLogLevel(LogLevel loglevel){
  auto ll = file_ll_;
	switch (loglevel) {
	case Debug:
		ll = log4cplus::DEBUG_LOG_LEVEL;
		break;
	case Info:
		ll = log4cplus::INFO_LOG_LEVEL;
		break;
	case Warning:
		ll = log4cplus::WARN_LOG_LEVEL;
		break;
	case Error:
		ll = log4cplus::ERROR_LOG_LEVEL;
		break;
	case Fatal:
		ll = log4cplus::FATAL_LOG_LEVEL;
		break;
	default:
		break;
	}
	if (ll == file_ll_) {
		return;
	}
	file_ll_ = ll;
	log4cplus::Logger client = log4cplus::Logger::getInstance(module_);
	client.setLogLevel(ll);
}

MyLogManager::~MyLogManager(){
  log4cplus::Logger::shutdown();
}

} // namespace log