#include "../../../command.hpp"
#include <iostream>

class MissionComments : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_SCAPI_AMC; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the contentId" << endl;
		string contentId;
		cin >> contentId;

		map<string, string> map;
		map["title"] = "gtav";
		map["contentId"] = contentId;

		return run("https://scapi.rockstargames.com/ugc/mission/comments", map);
	}
};

MissionComments g_mission_comments("getmissioncomments");