#include "command.hpp"
#include <format>

class GetLocationInfoFromIP : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["ipAddrStr"] = args[0];

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/GeoLocation.asmx/GetLocationInfoFromIP", 38, map);
	}
};

GetLocationInfoFromIP g_get_location_info_from_ip("getlocationinfoFromip");