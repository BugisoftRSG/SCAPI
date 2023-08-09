#include "../../../command.hpp"
#include <iostream>

class QueryContentCreators : command
{
	// No clue for what this is used and why but GetByUserId seems to be the only queryName

	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the content creator RID:" << endl; // TODO support multiple
		string contentCreators;
		cin >> contentCreators;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["queryName"] = "GetByUserId";
		map["queryParams"] = "{userids:[" + contentCreators + "]}"; // You can input here multiple seperated by a comma 
		map["offset"] = "0";
		map["count"] = "10";

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/QueryContentCreators", map);
	}
};

QueryContentCreators g_query_content_creators("querycontentcreators");