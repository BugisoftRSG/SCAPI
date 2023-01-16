#include "command.hpp"
#include <iostream>

class CopyLocalContent : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["contentId"] = "Qa2frulcv0u2eHmWg7KSHA";

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CopyContent", 42, map);
	}
};

CopyLocalContent g_copy_local_content("copylocalcontent");