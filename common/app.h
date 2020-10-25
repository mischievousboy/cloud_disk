#ifndef _H_APP_H_
#define _H_APP_H_

#include <string>

class App {
public:
  int Run();
  virtual ~App(){};

protected:
  std::string buff_;
  std::string module_;

private:
  virtual bool Init(){};
  virtual int RunInternal() = 0;
};
#endif //_H_APP_H_