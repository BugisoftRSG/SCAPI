#include "../../../command.hpp"
#include <iostream>

class GetAll : command
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

		cout << "Specify if isSystemClan (0 = no/1 = yes, -1 = any):" << endl;
		string isSystemClan;
		cin >> isSystemClan;

		cout << "Specify if isOpenClan (0 = no/1 = yes, -1 = any):" << endl;
		string isOpenClan;
		cin >> isOpenClan;

		cout << "Specify the search (Can be 0):" << endl;
		string search;
		cin >> search;

		cout << "Specify the sortMode (Can be 0, other values unknown yet):" << endl;
		string sortMode;
		cin >> sortMode;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pageIndex"] = pageIndex;
		map["pageSize"] = pageSize;
		map["isSystemClan"] = isSystemClan;
		map["isOpenClan"] = isOpenClan;
		map["search"] = search;
		map["sortMode"] = sortMode;

		return run("http://crews-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Clans.asmx/GetAll", map);
	}
};

GetAll g_get_all("getallcrews");