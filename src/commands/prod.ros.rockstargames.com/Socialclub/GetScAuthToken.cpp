#include "../../../command.hpp"
#include <format>

class GetScAuthToken : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;

		return run("http://prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/GetScAuthToken", map);
	}
};

GetScAuthToken g_get_sc_auth_token("getscauthtoken");