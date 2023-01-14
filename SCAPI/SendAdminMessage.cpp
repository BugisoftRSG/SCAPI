#include "command.hpp"
#include <format>

class SendAdminMessage : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["recipientsCsv"] = "SC+" + args[0];
		// "ros.publish":{"channel":"self","msg":{"gm.evt":{"e":"rockstar_message","d":{"msg":"%s  ~o~"}}}}
		map["message"] = R"("ros.publish":{"channel":"self","msg":{"gm.evt":{"e":"rockstar_message","d":{"msg":"Test"}}}})";
		map["ttlSeconds"] = "0";

		return run("http://prs-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Presence.asmx/MultiPostMessage", 42, map);
	}
};

SendAdminMessage g_send_admin_message("sendadminmsg");