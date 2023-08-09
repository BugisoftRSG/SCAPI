#include "../../../command.hpp"
#include <iostream>

class Kick : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the targetGamerHandle:" << endl;
		string targetGamerHandle;
		cin >> targetGamerHandle;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;
		map["targetGamerHandle"] = targetGamerHandle;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/Kick", map);
	}
};

Kick g_kick("kickfromcrew");