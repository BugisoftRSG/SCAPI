#include "command.hpp"
#include "ros_crypt.hpp"
#include <cpr/cpr.h>

ROSCrypt* command::m_launcher_ros = nullptr;
ROSCrypt* command::m_ros = nullptr;

command::command(const string& name) : m_name(name)
{
	m_launcher_ros = new ROSCrypt(true);
	m_ros = new ROSCrypt(false);

	g_commands[hash<string>()(name)] = this;
}

command* command::get(int command)
{
	return g_commands[command];
}

string command::run_raw(const string url, const string queryString, bool launcher)
{
	auto api = launcher ? m_launcher_ros : m_ros;

	string absolutePath = url.substr(url.find('/', url.find("://") + 3));

	Botan::AutoSeeded_RNG rng;
	auto challenge = rng.random_vec(8);

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Header
		{
			{"ros-SecurityFlags", "239" },
			{"ros-SessionTicket", SESSION_TICKET},
			{"ros-Challenge", Botan::base64_encode(challenge)},
			{"ros-HeadersHmac", Botan::base64_encode(api->HeadersHmac(challenge, "POST", absolutePath, SESSION_KEY, SESSION_TICKET)) },
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"User-Agent", api->GetROSVersionString()},
		},
		cpr::Body{ queryString });

	return response.text;
}

string command::run(const string url, map<string, string> map)
{
	auto service_type = this->get_service_type();

	if (service_type == PROD_ROS || service_type == PROD_ROS_LAUNCHER)
	{
		auto api = service_type == SERVICE_TYPE::PROD_ROS_LAUNCHER ? m_launcher_ros : m_ros;

		auto params = api->BuildPostString(map);

		string queryString = api->EncryptROSData(params, SESSION_KEY);

		string response = run_raw(url, queryString, service_type == SERVICE_TYPE::PROD_ROS_LAUNCHER);

		if (!response.contains("Internal server error"))
			return api->DecryptROSData(response.c_str(), response.size(), SESSION_KEY);
		else {
			return response;
		}
	}
	else if (service_type == PROD_SCAPI_AMC)
	{
		cpr::Parameters params;
		for (const auto& entry : map) {
			params.Add(cpr::Parameter(entry.first, entry.second));
		}

		cpr::Response response = cpr::Get(
		cpr::Url{ url },
		cpr::Header
		{
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"X-AMC", "true"},
			{"X-Requested-With", "XMLHttpRequest"}
		},
		cpr::Parameters{ params });

		return response.text;
	}

	return "";
}

string command::run(const string url, string params)
{
	string queryString = m_ros->EncryptROSData(params, SESSION_KEY);

	string response = run_raw(url, queryString);

	if (!response.contains("Internal server error"))
		return m_ros->DecryptROSData(response.c_str(), response.size(), SESSION_KEY);
	else {
		return response;
	}
}

string command::run_anonymous(const string& url, map<string, string> map)
{
	string queryString = m_ros->EncryptROSData(m_ros->BuildPostString(map));

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Header
		{
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"User-Agent", m_ros->GetROSVersionString()}
		},
		cpr::Body{ queryString });

	return m_ros->DecryptROSData(response.text.c_str(), response.text.size(), "");
}