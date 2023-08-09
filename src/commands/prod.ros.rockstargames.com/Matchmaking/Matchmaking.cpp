#include "../../../command.hpp"
#include <format>

class Matchmaking : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;
		map["availableSlots"] = "1";
		map["filterName"] = "Group";
		// filterParamsJson={"GAME_MODE":0,"MMATTR_REGION":0}
		map["filterParamsJson"] = R"({"GAME_MODE":0,"MMATTR_MM_GROUP_2":30,"MMATTR_REGION":3})";
		map["maxResults"] = "999999";

		return run("http://mm-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/matchmaking.asmx/Find", map);
	}
};

Matchmaking g_matchmaking("matchmaking");