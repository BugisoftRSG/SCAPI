#include "command.hpp"
#include <format>

class SendEmail : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["userIds"] = args[0];
		map["message"] = std::format(R"({{"email":{{"gh":"8M6BuXUBAAAAAAAAAAAAAA==","sb":"EmailSB","cn":"EmailCN"}}}})");
		map["tagsCsv"] = "gta5email";
		map["ttlSeconds"] = "2592000";

		return run("http://inbox-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Inbox.asmx/PostMessageToRecipients", 44, map);
	}
};

SendEmail g_send_email("sendemail");