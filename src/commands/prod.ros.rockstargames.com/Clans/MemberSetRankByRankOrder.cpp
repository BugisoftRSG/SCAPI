#include "../../../command.hpp"
#include <iostream>

class MemberSetRankByRankOrder : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the targetGamerHandle:" << endl;
		string targetGamerHandle;
		cin >> targetGamerHandle;

		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the rankOrder:" << endl;
		string rankOrder;
		cin >> rankOrder;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["targetGamerHandle"] = targetGamerHandle;
		map["clanId"] = clanId;
		map["rankOrder"] = rankOrder;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/MemberSetRankByRankOrder", map); // Title or Environment not supported
	}
};

MemberSetRankByRankOrder g_member_set_rank_by_rank_order("membersetrankbyrankorder");