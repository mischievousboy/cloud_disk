#include <file_until.h>
#include <unistd.h>

namespace file_util{

std::string GetPwd(){
  char path[1024] = {0};
  getcwd(path, 1024);
  return std::string(path);
}

bool FileExist(const std::string &path) {
  return access(path.c_str(), F_OK) == 0;
}
}