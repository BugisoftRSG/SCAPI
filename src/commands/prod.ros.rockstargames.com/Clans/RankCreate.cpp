#include "../../../command.hpp"
#include <iostream>

class RankCreate : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the rankName (0-4):" << endl; //?
		string rankName;
		cin >> rankName;

		cout << "Specify the initialSystemFlags:" << endl;
		string initialSystemFlags;
		cin >> initialSystemFlags;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["clanId"] = clanId;
		map["rankName"] = rankName;
		map["initialSystemFlags"] = initialSystemFlags;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/RankCreate", map); // Error: OutOfRange -> ClanRankLength
	}
};

RankCreate g_rank_create("createcrewrank");