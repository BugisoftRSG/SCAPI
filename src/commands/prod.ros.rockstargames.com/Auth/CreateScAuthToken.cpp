#include "../../../command.hpp"
#include <iostream>

class CreateScAuthToken : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{

		map<string, string> map;
		map["ticket"] = TICKET;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/auth.asmx/CreateTicketScAuthToken2", map);
	}
};

CreateScAuthToken g_create_sc_auth_token("createscauthtoken");