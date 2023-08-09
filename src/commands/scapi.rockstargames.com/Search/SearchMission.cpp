#include "../../../command.hpp"
#include <iostream>

class SearchMission : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_SCAPI_AMC; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["title"] = "gtav";
		map["platform"] = "pc";
		map["includeCommentCount"] = "false";

		return run("https://scapi.rockstargames.com/search/mission", map);
	}
};

SearchMission g_search_mission("searchmission");