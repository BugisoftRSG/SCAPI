#include "command.hpp"
#include "ros_crypt.hpp"
#include "cpr/cpr.h"

command::command(const std::string& name) :
	m_name(name)
{
	g_commands[std::hash<std::string>()(name)] = this;
}

command* command::get(int command)
{
	return g_commands[command];
}

std::string command::run(const std::string& url, int offset, std::map<std::string, std::string> map)
{
	std::string queryString = EncryptROSData(BuildPostString(map), SESSION_KEY);

	Botan::AutoSeeded_RNG rng;
	auto challenge = rng.random_vec(8);

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Header
		{ 
			{"ros-SecurityFlags", "239" },
			{"ros-SessionTicket", SESSION_TICKET},
			{"ros-Challenge", Botan::base64_encode(challenge)},
			{"ros-HeadersHmac", Botan::base64_encode(HeadersHmac(challenge, "POST", url.substr(offset), SESSION_KEY, SESSION_TICKET))},
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"User-Agent", GetROSVersionString()}
		},
		cpr::Body{ queryString });

	return DecryptROSData(response.text.c_str(), response.text.size(), SESSION_KEY);
}

std::string command::run(const std::string& url, int offset, std::string params)
{
	std::string queryString = EncryptROSData(params, SESSION_KEY);

	Botan::AutoSeeded_RNG rng;
	auto challenge = rng.random_vec(8);

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Header
		{
			{"ros-SecurityFlags", "239" },
			{"ros-SessionTicket", SESSION_TICKET},
			{"ros-Challenge", Botan::base64_encode(challenge)},
			{"ros-HeadersHmac", Botan::base64_encode(HeadersHmac(challenge, "POST", url.substr(offset), SESSION_KEY, SESSION_TICKET))},
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"User-Agent", GetROSVersionString()}
		},
		cpr::Body{ queryString });

	return DecryptROSData(response.text.c_str(), response.text.size(), SESSION_KEY);
}

std::string command::run_create_ticket(const std::string& url, int offset, std::map<std::string, std::string> map)
{
	std::string queryString = EncryptROSData(BuildPostString(map));

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Header
		{
			{"Content-Type", "application/x-www-form-urlencoded; charset=utf-8"},
			{"User-Agent", GetROSVersionString()}
		},
		cpr::Body{ queryString });

	return DecryptROSData(response.text.c_str(), response.text.size(), SESSION_KEY);
}