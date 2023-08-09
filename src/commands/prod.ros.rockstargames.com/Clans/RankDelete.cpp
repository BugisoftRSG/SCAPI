#include "../../../command.hpp"
#include <iostream>

class RankDelete : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the rankId:" << endl;
		string rankId;
		cin >> rankId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;
		map["rankId"] = rankId;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/RankDelete", map);
	}
};

RankDelete g_rank_delete("deletecrewrank");