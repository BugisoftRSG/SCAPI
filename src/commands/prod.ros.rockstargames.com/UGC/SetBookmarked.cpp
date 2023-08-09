#include "../../../command.hpp"
#include <iostream>

class SetBookmarked : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the content id:" << endl;
		string contentId;
		cin >> contentId;

		cout << "Specify if it should be bookmarked (true/false):" << endl;
		string bookmarked;
		cin >> bookmarked;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["contentId"] = contentId;
		map["bookmarked"] = bookmarked;

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/SetBookmarked", map);
	}
};

SetBookmarked g_set_bookmarked("setbookmarked");