#include "command.hpp"
#include <iostream>

class QueryContent : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		std::cout << TICKET << std::endl;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["queryName"] = "GetLatestVersionByContentId";
		map["queryParams"] = "{contentids:[\"" + args[0] + "\"], lang: ['en', 'fr', 'de', 'it', 'es', 'pt', 'pl', 'ru', 'es-mx'] }";
		map["offset"] = "0";
		map["count"] = "500";

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/QueryContent", 42, map);
	}
};

QueryContent g_query_content("querycontent");