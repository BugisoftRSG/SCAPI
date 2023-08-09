#include "../../../command.hpp"
#include <iostream>

class BlockCommunications : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the rockstarId:" << endl;
		string rockstarId;
		cin >> rockstarId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["rockstarId"] = rockstarId;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/Block", map); // Missleading name, dont have to be a friend
	}
};

BlockCommunications g_block_communications("blockcoms");