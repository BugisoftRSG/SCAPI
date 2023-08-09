#include "../../../command.hpp"
#include <iostream>

class GetDesc : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetDesc", map);
	}
};

GetDesc g_get_desc("getcrewdesc");