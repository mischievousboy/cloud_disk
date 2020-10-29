#include "mylog.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/spi/loggingevent.h>

#include "mylog_manager.h"



namespace Mylog {

LogManager* LogManager::Create(){
	return new MyLogManager();
}


std::string MyLogMessage::module_name_ = "log";

MyLogMessage::MyLogMessage(const char* file, int line, const char* func,
	LogLevel level)
	: file_(file), line_(line), func_(func), level_(level) {}

MyLogMessage::~MyLogMessage() {
  std::ostringstream head;
  std::string msg_str;
  std::string file_name = file_;
  log4cplus::Logger mylog = log4cplus::Logger::getInstance(module_name_);
  size_t pos = file_name.find_last_of("/");
  if (pos != std::string::npos) {
    file_name = file_name.substr(pos + 1);
  }

  head << file_name << ":" << line_ << "|" << func_ << "|";

  msg_str += head.str();
  msg_str += stream_.str();
  char *msg = (char *)msg_str.c_str();
  switch (level_) {
  case Debug:
    LOG4CPLUS_DEBUG_STR(mylog, msg);
    break;
  case Info:
    LOG4CPLUS_INFO_STR(mylog, msg);
    break;
  case Warning: {
    LOG4CPLUS_WARN_STR(mylog, msg);
  } break;
  case Error: {
    LOG4CPLUS_ERROR_STR(mylog, msg);
  } break;
  case Fatal: {
    LOG4CPLUS_FATAL_STR(mylog, msg);
  } break;
  }
}
} // namespace log