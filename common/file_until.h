#ifndef _H_FILE_UNTIL_H_
#define _H_FILE_UNTIL_H_

#include <string>

namespace file_util{

std::string GetPwd();
bool FileExist(const std::string& path);
}
#endif //_H_FILE_UNTIL_H_