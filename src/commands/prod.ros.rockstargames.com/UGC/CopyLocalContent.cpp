#include "../../../command.hpp"
#include <iostream>

class CopyLocalContent : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["contentId"] = "Qa2frulcv0u2eHmWg7KSHA"; // TODO

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CopyContent", map);
	}
};

CopyLocalContent g_copy_local_content("copylocalcontent");