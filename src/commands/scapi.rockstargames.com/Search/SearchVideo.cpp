#include "../../../command.hpp"
#include <iostream>

class SearchVideo : command
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
		//map["sort"] = "date";
		//map["dateRange"] = "any";
		//map["creatorRockstarId"] = "12345678";
		//map["pageSize"] = "1";
		//map["filter"] = "friends";

		return run("https://scapi.rockstargames.com/search/video", map);
	}
};

SearchVideo g_search_video("searchvideo");