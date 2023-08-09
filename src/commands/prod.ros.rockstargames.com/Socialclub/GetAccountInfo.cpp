#include "../../../command.hpp"
#include <format>

class GetAccountInfo : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;

		return run("http://prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/GetAccountInfo", map);
	}
};

GetAccountInfo g_get_account_info("getaccountinfo");