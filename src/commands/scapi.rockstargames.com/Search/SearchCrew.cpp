#include "../../../command.hpp"
#include <iostream>

class SearchCrew : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_SCAPI_AMC; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["title"] = "gtav";
		map["platform"] = "pc";
		map["includeCommentCount"] = "false";
		//map["searchTerm"] = "looping";
		//map["sort"] = "membercount";
		//map["dateRange"] = "any";
		//map["pageSize"] = "1";
		//map["crewtype"] = "rockstar";

		return run("https://scapi.rockstargames.com/search/crew", map);
	}
};

SearchCrew g_search_crew("searchcrew");