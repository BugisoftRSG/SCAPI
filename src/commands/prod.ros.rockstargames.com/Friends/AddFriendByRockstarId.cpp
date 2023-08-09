#include "../../../command.hpp"
#include <iostream>

class InviteByRockstarId : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the rockstar id:" << endl;
		string rockstarId;
		cin >> rockstarId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["rockstarId"] = rockstarId;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/InviteByRockstarId", map); // This name is missleading since you doesnt actually "invite" a friend instead send a friend request
	}
};

InviteByRockstarId g_invite_by_rockstarid("addfriendbyrid");