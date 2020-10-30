#include "register.h"

#include <iostream>
#include <log/mylog.h>
#include <database/database.h>

#define  MARKB "'"
#define  MARKE "' "

#define CREATITEM(value) MARKB << value.c_str() << MARKE


namespace reg {
	bool MyRegister::Parse(const std::string& context) {
		try {
			x2struct::X::loadjson(context, user_info_,false);
		}
		catch (const std::exception& e) {
			LOGERROR << "reg context parse error!";
			return false;
		}
		return true;
	}

	int MyRegister::StoreDB() {
		sql::DataBaseManager* db = sql::DataBaseManager::GetInstance();
		std::ostringstream ostream;
		ostream << "select* from user_info where user_name = " << user_info_.name;
		if(db->Exec(ostream.str())){
			std::vector<std::string> res = db->GetResults();
			if (res.size() != 0) {
				return -2; //用户已存在
			}
			ostream.clear();
			ostream << "insert into user_info (user_name, nick_name, password, phone, email) values ("
				<< CREATITEM(user_info_.name) << CREATITEM(user_info_.nick_name) << CREATITEM(user_info_.pwd) << CREATITEM(user_info_.phone) << CREATITEM(user_info_.email);
			if (!db->Exec(ostream.str()))
				return -1;
		}

		return 0;
	}
}