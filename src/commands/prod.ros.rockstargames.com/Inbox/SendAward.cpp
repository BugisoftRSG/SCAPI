#include "../../../command.hpp"
#include <format>
#include <iostream>

class SendAwardMsg : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the target rid:" << endl;
		string target_rid;
		cin >> target_rid;

		cout << "Specify the sender name:" << endl;
		string senderGamerTag;
		cin >> senderGamerTag;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["recipientsCsv"] = target_rid;
		map["message"] = format(R"("ros.publish":{{"channel":"friends","msg":{{"gm.evt":{{"e":"StatUpdate","d":{{"stat":1488317176,"from":"{}","ival":100}}}}}}}})", senderGamerTag);
		map["ttlSeconds"] = "0";

		return run("http://prs-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Presence.asmx/MultiPostMessage", map);
	}
};

SendAwardMsg g_send_award_msg("sendawardmessage");