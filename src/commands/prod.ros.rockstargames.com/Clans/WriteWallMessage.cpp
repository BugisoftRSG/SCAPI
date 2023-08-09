#include "../../../command.hpp"
#include <iostream>

class WriteWallMessage : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the message:" << endl;
		string message;
		cin >> message;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;
		map["message"] = message;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/WriteWallMessage", map); // Internal Server Error, no longer in use?
	}
};

WriteWallMessage write_wall_message("writecrewwallmessage");