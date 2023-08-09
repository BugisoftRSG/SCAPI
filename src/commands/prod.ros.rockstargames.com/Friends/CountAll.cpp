#include "../../../command.hpp"
#include <iostream>

class CountAll : command
{
	using command::command;

	/*
	* Response:
	* b -> ?
	* f -> Friend Count
	* ir -> Friend Requests Received (See: getfriendrequestssent)
	* is -> Friend Requests Sent (See: 
	*/

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/CountAll", map);
	}
};

CountAll g_count_all_friends("countallfriends");