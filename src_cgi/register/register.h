#ifndef _H_REGISTER_H_
#define _H_REGISTER_H_

#include <x2struct/x2struct.hpp>


namespace reg{
	struct UserInfo {
		std::string pwd;
		std::string name;
		std::string nick_name;
		std::string email;
		std::string phone;
		XTOSTRUCT(M(pwd, name, nick_name), O(email, phone));
	};
	class MyRegister {
	public:
		bool Parse(const std::string& context);
		int StoreDB();
	
	private:
		UserInfo user_info_;
	};
}

#endif //_H_REGISTER_H_
