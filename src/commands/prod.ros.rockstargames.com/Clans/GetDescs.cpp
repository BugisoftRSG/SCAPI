#include "../../../command.hpp"
#include <iostream>

class GetDescs : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanIdsCsv (ex.: 12345,56789):" << endl;
		string clanIdsCsv;
		cin >> clanIdsCsv;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanIdsCsv"] = clanIdsCsv;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetDescs", map);
	}
};

GetDescs g_get_descs("getcrewdescs");