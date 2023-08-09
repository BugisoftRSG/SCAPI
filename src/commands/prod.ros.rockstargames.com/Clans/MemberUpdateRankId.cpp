#include "../../../command.hpp"
#include <iostream>

class MemberUpdateRankId : command
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

		cout << "Specify if promote (true/false):" << endl;
		string promote;
		cin >> promote;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["targetGamerHandle"] = targetGamerHandle;
		map["clanId"] = clanId;
		map["promote"] = promote;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/MemberUpdateRankId", map); // Invalid Argument for some reason
	}
};

MemberUpdateRankId g_member_update_rank_id("memberupdaterankid");