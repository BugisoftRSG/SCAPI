#include "../../../command.hpp"
#include <format>
#include "AccountInfo.hpp"

class UpdateAccount : command
{
	using command::command;

	inline AccountInfo GetAccountInfo()
	{
		string data = command::get(hash<string>()("getaccountinfo"))->execute({});
		pugi::xml_document doc;
		AccountInfo accountInfo{};

		if (doc.load_string(data.c_str()))
		{
			auto path = doc.select_node("//Response/RockstarAccount");
			auto node = path.node();

			accountInfo.m_rockstar_id = node.child("RockstarId").text().as_uint();
			accountInfo.m_age = node.child("Age").text().as_int();
			accountInfo.m_avatar_url = node.child("AvatarUrl").text().as_string();
			accountInfo.m_country_code = node.child("CountryCode").text().as_string();
			accountInfo.m_date_of_birth = node.child("Dob").text().as_string();
			accountInfo.m_email = node.child("Email").text().as_string();
			accountInfo.m_is_approx_dob = node.child("IsApproxDob").text().as_bool(); // TODO
			accountInfo.m_language_code = node.child("LanguageCode").text().as_string();
			accountInfo.m_nickname = node.child("Nickname").text().as_string();
			accountInfo.m_last_portal_login_date = node.child("LastPortalLoginDate").text().as_string();
		}

		return accountInfo;
	}

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		AccountInfo accountInfo = GetAccountInfo();

		cout << "Specify the email:" << endl;
		string email;
		cin >> email;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["avatarUrl"] = "null"; // if this invalid, it will be skipped, you can't just put a .jpg or .png there
		map["countryCode"] = accountInfo.m_country_code;
		map["dob"] = accountInfo.m_date_of_birth;
		map["email"] = email;
		map["isApproxDob"] = accountInfo.m_is_approx_dob ? "True" : "False";
		map["languageCode"] = accountInfo.m_language_code;
		map["nickname"] = accountInfo.m_nickname;
		map["phone"] = "";
		map["zipCode"] = "";


		return run("http://prod.ros.rockstargames.com/gta5/11/gameservices/socialclub.asmx/UpdateAccount", map);
	}
};

UpdateAccount g_update_account("updateaccount");