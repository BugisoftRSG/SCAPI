#include "../../../command.hpp"
#include <iostream>

class AwardAchievementProgress : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the achievementId:" << endl;
		string achievementId;
		cin >> achievementId;

		cout << "Specify the achievementProgress:" << endl;
		string achievementProgress;
		cin >> achievementProgress;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["achievementId"] = achievementId;
		map["achievedOffline"] = "False";

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/achievements.asmx/AwardAchievementProgress", map);
	}
};

AwardAchievementProgress g_award_achievement_progress("awardachievementprogress");