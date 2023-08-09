#include "../../../command.hpp"
#include <iostream>

class GetFeudStats : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the since ((lastday, lastweek, last2weeks, lastmonth, last3months, last6months, alltime) / 0):" << endl;
		string since;
		cin >> since;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;
		map["since"] = since;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetFeudStats", map); // TODO Internal Server Error (no longer in use?)
	}
};

GetFeudStats g_get_feud_stats("getfeudstats");