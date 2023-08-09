#include "../../../command.hpp"
#include <iostream>
//#include <fstream>

class ReadGameConfig : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;

		//ofstream of("readgameconfig.txt");
		//of << run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ProfileStatGroups.asmx/ReadGameConfig", map);

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ProfileStatGroups.asmx/ReadGameConfig", map); // Response text is too large for console
	}
};

ReadGameConfig g_read_game_config("readgameconfig");