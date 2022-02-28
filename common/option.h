//
// Created by huihu on 2022/2/25.
//

#ifndef CLOUD_DISK_OPTION_H
#define CLOUD_DISK_OPTION_H

#include <vector>
#include <string>
#include <map>

class Option {
public:
    Option();
    void Decode(int argc,char* argv[]);
    void parse(const std::string& str);
    const std::string& getParam(const std::string& key,const std::string& def ="") const;
private:
    std::vector<std::string> single_command_;
    std::map<std::string,std::string> param_;
};


#endif//CLOUD_DISK_OPTION_H
