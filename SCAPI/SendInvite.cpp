#include "command.hpp"
#include <format>

class SendInvite : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["recipientsCsv"] = "SC+" + args[2];
		map["message"] = std::format(R"({{"ros.mp.invite":{{"h":"SC+{}","n":"{}","s":"{}"}}}})", args[0], args[1], args[3]);
		map["ttlSeconds"] = "0";

		return run("http://prs-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Presence.asmx/MultiPostMessage", 42, map);
	}
};

SendInvite g_send_invite("sendinvite");