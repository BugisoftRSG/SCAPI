#include "../../../command.hpp"
#include <iostream>

class GetPrimaryClans : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the gamerHandlesCsv (ex.: 123456,789900):" << endl;
		string gamerHandlesCsv;
		cin >> gamerHandlesCsv;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["gamerHandlesCsv"] = gamerHandlesCsv;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetPrimaryClans", map); // Exception, no further info
	}
};

GetPrimaryClans g_get_primary_clans("getprimaryclans");