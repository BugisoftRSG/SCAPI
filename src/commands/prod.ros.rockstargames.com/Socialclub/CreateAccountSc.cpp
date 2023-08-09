#include "../../../command.hpp"
#include <format>

class CreateAccountSc : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the country code" << endl;
		string countryCode;
		cin >> countryCode;

		cout << "Specify the date of bearth (dd/mm/yyyy)" << endl;
		string dob;
		cin >> dob;

		cout << "Specify the email" << endl;
		string email;
		cin >> email;

		cout << "Specify the language code" << endl;
		string languageCode;
		cin >> languageCode;

		cout << "Specify the nickname" << endl;
		string nickname;
		cin >> nickname;

		cout << "Specify the password" << endl;
		string password;
		cin >> password;

		map<string, string> map;
		map["acceptNewsletter"] = "false";
		map["avatarUrl"] = "https://prod-avatars.akamaized.net/stock-avatars/n/GTAO/CASINO_2.png";
		map["countryCode"] = countryCode;
		map["dob"] = dob;
		map["email"] = email;
		map["isApproxDob"] = "False";
		map["languageCode"] = languageCode;
		map["nickname"] = nickname;
		map["password"] = password;
		map["phone"] = "";
		map["platform"] = "pcros";
		map["zipCode"] = "1379800";


		return run("http://prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/CreateAccountSc", map);
	}
};

CreateAccountSc g_create_account_sc("createaccountsc");