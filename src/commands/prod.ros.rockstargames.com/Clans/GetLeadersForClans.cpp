#include "../../../command.hpp"
#include <iostream>

class GetLeadersForClans : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanIdsCsv (ex.: 123456,789801):" << endl;
		string clanIdsCsv;
		cin >> clanIdsCsv;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanIdsCsv"] = clanIdsCsv;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetLeadersForClans", map);
	}
};

GetLeadersForClans g_get_leaders_for_clans("getleadersforclans");