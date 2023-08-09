#include "../../../command.hpp"
#include <iostream>

class GetMine : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the pageIndex:" << endl;
		string pageIndex;
		cin >> pageIndex;

		cout << "Specify the pageSize:" << endl;
		string pageSize;
		cin >> pageSize;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pageIndex"] = pageIndex;
		map["pageSize"] = pageSize;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetMine", map);
	}
};

GetMine g_get_mine("getmycrew");