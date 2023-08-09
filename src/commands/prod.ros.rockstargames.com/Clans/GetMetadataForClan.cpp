#include "../../../command.hpp"
#include <iostream>

class GetMetadataForClan : command
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

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pageIndex"] = pageIndex;
		map["pageSize"] = pageSize;
		map["clanId"] = clanId;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetMetadataForClan", map);
	}
};

GetMetadataForClan g_get_metadata_for_clan("getcrewmetadata");