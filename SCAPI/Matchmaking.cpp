#include "command.hpp"
#include <format>

class Matchmaking : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = TICKET;
		map["availableSlots"] = "1";
		map["filterName"] = "Group";
		// filterParamsJson={"GAME_MODE":0,"MMATTR_REGION":0}
		map["filterParamsJson"] = R"({"GAME_MODE":0,"MMATTR_MM_GROUP_2":30,"MMATTR_REGION":3})";
		map["maxResults"] = "999999";

		return run("http://mm-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/matchmaking.asmx/Find", 41, map);
	}
};

Matchmaking g_matchmaking("matchmaking");