#include "../../../command.hpp"
#include <iostream>

class DeleteContent : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the content id:" << endl;
		string contentId;
		cin >> contentId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["contentType"] = "gta5mission";
		map["contentId"] = contentId;
		map["deleted"] = "true";

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/SetDeleted", map);
	}
};

DeleteContent g_delete_content("deletecontent");