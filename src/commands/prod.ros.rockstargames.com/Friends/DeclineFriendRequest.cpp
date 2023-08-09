#include "../../../command.hpp"
#include <iostream>

class DeclineFriendRequest : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the rockstarId:" << endl;
		string rockstarId;
		cin >> rockstarId;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["rockstarId"] = rockstarId;

		return run("http://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Friends.asmx/DeclineInvite", map); // Missleading name, friend request not session invite
	}
};

DeclineFriendRequest g_decline_friend_request("declinefriendrequest");