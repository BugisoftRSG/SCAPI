#include "../../../command.hpp"
#include <format>

class SendInvite : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the sender rid:" << endl;
		string sender_rid;
		cin >> sender_rid;
		cout << "Specify the sender name:" << endl;
		string sender_name;
		cin >> sender_name;
		cout << "Specify the target rid:" << endl;
		string target_rid;
		cin >> target_rid;
		cout << "Specify the session info:" << endl;
		string session_info;
		cin >> session_info;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["recipientsCsv"] = "SC+" + target_rid;
		map["message"] = format(R"("ros.mp.invite":{{"h":"SC+{}","n":"{}","s":"{}"}})", sender_rid, sender_name, session_info);
		map["ttlSeconds"] = "0";

		return run("http://prs-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Presence.asmx/MultiPostMessage", map);
	}
};

SendInvite g_send_invite("sendinvite");