#include "../../../command.hpp"
#include <iostream>

class GetAchievementDefinitions : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the pageIndex:" << endl;
		string firstAchievementId;
		cin >> firstAchievementId;

		cout << "Specify the maxDefinitions:" << endl;
		string maxDefinitions;
		cin >> maxDefinitions;

		cout << "Specify the locale (ex. en-US):" << endl;
		string locale;
		cin >> locale;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["firstAchievementId"] = firstAchievementId;
		map["maxDefinitions"] = maxDefinitions;
		map["locale"] = locale;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/achievements.asmx/GetAchievementDefinitions", map);
	}
};

GetAchievementDefinitions g_get_achievement_definitions("getachievementdefinitions");