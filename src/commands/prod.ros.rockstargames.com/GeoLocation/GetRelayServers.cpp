#include "../../../command.hpp"
#include <iostream>

class GetRelayServers : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the ipAddrStr:" << endl;
		string ipAddrStr;
		cin >> ipAddrStr;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["ipAddrStr"] = ipAddrStr;
		map["secure"] = "false";

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/GeoLocation.asmx/GetRelayServers", map);
	}
};

GetRelayServers g_get_relay_servers("getrelayservers");