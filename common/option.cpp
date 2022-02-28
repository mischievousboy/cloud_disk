//
// Created by huihu on 2022/2/25.
//

#include "option.h"

using namespace std;

void Option::Decode(int argc, char **argv) {
    param_.clear();
    string tmp;
    for (int i = 0; i < argc; ++i) {
        tmp = argv[i];
        if (tmp.length() > 2 && tmp.find("--") == 0) {
            parse(tmp);
        } else
            single_command_.push_back(tmp);
    }
}
void Option::parse(const string &str) {
    string::size_type pos = str.find('=');
    if (pos != string::npos) {
        param_[str.substr(2, pos - 2)] = str.substr(pos + 1);
    } else
        param_[str.substr(2)];
}
const std::string &Option::getParam(const string &key, const string &def) const {
    auto iter = param_.find(key);
    if (iter != param_.cend())
        return iter->second;
    return def;
}
Option::Option() = default;
