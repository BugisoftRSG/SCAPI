#include "../../../command.hpp"
#include <iostream>

class AwardAchievement : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the achievementId:" << endl;
		string achievementId;
		cin >> achievementId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["achievementId"] = achievementId;
		map["achievedOffline"] = "False";

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/achievements.asmx/AwardAchievement", map);
	}
};

AwardAchievement g_award_achievement("awardachievement");