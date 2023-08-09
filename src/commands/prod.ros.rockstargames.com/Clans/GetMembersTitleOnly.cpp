#include "../../../command.hpp"
#include <iostream>

class GetCrewMembersTitleOnly : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the pageIndex:" << endl;
		string pageIndex;
		cin >> pageIndex;

		cout << "Specify the pageSize:" << endl;
		string pageSize;
		cin >> pageSize;

		cout << "Specify the clanId:" << endl;
		string clanId;
		cin >> clanId;

		cout << "Specify the clanName:" << endl;
		string clanName;
		cin >> clanName;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pageIndex"] = pageIndex;
		map["pageSize"] = pageSize;
		map["clanId"] = clanId;
		map["clanName"] = clanName;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetMembersTitleOnly", map);
	}
};

GetCrewMembersTitleOnly g_get_crew_members_title_only("getcrewmemberstitleonly");