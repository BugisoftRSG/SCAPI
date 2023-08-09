#include "../../../command.hpp"
#include <iostream>

class GetFriends : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the page index:" << endl;
		string pageIndex;
		cin >> pageIndex;

		cout << "Specify the page size:" << endl;
		string pageSize;
		cin >> pageSize;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pageIndex"] = pageIndex;
		map["pageSize"] = pageSize;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/GetFriends", map);
	}
};

GetFriends g_get_friends("getfriends");