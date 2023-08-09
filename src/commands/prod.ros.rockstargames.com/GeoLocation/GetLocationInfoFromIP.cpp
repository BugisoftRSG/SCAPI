#include "../../../command.hpp"

class GetLocationInfoFromIP : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the ip:" << endl;
		string target_rid;
		cin >> target_rid;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["ipAddrStr"] = target_rid;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/GeoLocation.asmx/GetLocationInfoFromIP", map);
	}
};

GetLocationInfoFromIP g_get_location_info_from_ip("getlocationinfofromip");