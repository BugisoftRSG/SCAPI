#include "../../../command.hpp"
#include <format>

class LinkAccount2 : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the email:" << endl;
		string email;
		cin >> email;

		cout << "Specify the nickname:" << endl;
		string nickname;
		cin >> nickname;

		cout << "Specify the password:" << endl;
		string password;
		cin >> password;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["email"] = email;
		map["nickname"] = nickname;
		map["password"] = password;

		return run("http://prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/LinkAccount2", map);
	}
};

LinkAccount2 g_link_account_2("linkaccount2");