#include "../../../command.hpp"
#include <iostream>

class CheckText : command
{
	// Tested from UGC Creator -> Job Title

	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the text to check:" << endl;
		string text;
		cin >> text;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["languageCode"] = ""; // Can be empty
		map["contentName"] = ""; // Can be empty
		map["description"] = text; // Text to check
		map["tagCsv"] = ""; // Can be empty

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CheckText", map);
	}
};

CheckText g_check_text("checktext");