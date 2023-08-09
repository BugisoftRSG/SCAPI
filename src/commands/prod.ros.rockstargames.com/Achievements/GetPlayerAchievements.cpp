#include "../../../command.hpp"
#include <iostream>

class GetPlayerAchievements : command
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
		map["playerRockstarId"] = rockstarId;
		map["crossTitleId"] = "11";
		map["crossTitleName"] = "gta5";
		map["crossTitlePlatformName"] = "pcros";

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/achievements.asmx/GetPlayerAchievements", map);
	}
};

GetPlayerAchievements g_get_player_achievements("getplayerachievements");