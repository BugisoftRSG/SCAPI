#include "../../../command.hpp"
#include <iostream>

class InviteByNickname : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the nickname:" << endl;
		string nickname;
		cin >> nickname;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["nickName"] = nickname;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/InviteByNickname", map); // This name is missleading since you doesnt actually "invite" a friend instead send a friend request
	}
};

InviteByNickname g_invite_by_nickname("addfriendbyname");