#include "../../../command.hpp"

class CreateP2PCertificate : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the pubKey" << endl;
		string pubKey;
		cin >> pubKey;

		cout << "Specify the sig" << endl;
		string sig;
		cin >> sig;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["pubKeyCurve"] = "nistP256";
		map["pubKey"] = pubKey;
		map["hashAlg"] = "SHA256";
		map["sig"] = sig;

		return run("https://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/auth.asmx/CreateP2PCertificate", map);
	}
};

CreateP2PCertificate g_create_p2p_certificate("createp2pcertificate");