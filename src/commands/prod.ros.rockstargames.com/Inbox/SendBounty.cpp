#include "../../../command.hpp"
#include <format>

class BountyCompletedMsg : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the target rid:" << endl;
		string target_rid;
		cin >> target_rid;

		cout << "Specify the tl31FromGamerTag:" << endl;
		string tl31FromGamerTag;
		cin >> tl31FromGamerTag;

		cout << "Specify the tl31TargetGamerTag:" << endl;
		string tl31TargetGamerTag;
		cin >> tl31TargetGamerTag;

		cout << "Specify the iOutcome:" << endl;
		string iOutcome;
		cin >> iOutcome;

		cout << "Specify the iCash:" << endl;
		string iCash;
		cin >> iCash;

		cout << "Specify the iRank:" << endl;
		string iRank;
		cin >> iRank;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["userIds"] = target_rid;
		map["message"] = format(R"({{"gm.evt":{{"e":"bounty","d":{{"Ft":"{}","Tt":"{}","o":{},"c":{},"r":{},"t":1690670145566}}}}}})", tl31FromGamerTag, tl31TargetGamerTag, iOutcome, iCash, iRank);
		map["tagsCsv"] = "gta5";
		map["ttlSeconds"] = "1800";
			
		return run("http://inbox-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Inbox.asmx/PostMessageToRecipients", map);
	}
};

BountyCompletedMsg g_send_bounty_completed_msg("sendbountycompletedmsg");