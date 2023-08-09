#include "../../../command.hpp"
#include <format>

class ReadAllStats : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		map<string, string> map;
		map["ticket"] = TICKET;

		return run("https://ps-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/ReadAllStats", map); // loads infinite?
	}
};

ReadAllStats g_read_all_stats("readallstats");