#include "../../../command.hpp"
#include "pugixml.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

class CreateTicket : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	inline void request_socialclub_data(unordered_map<string, string>& env_vars)
	{
		cout << "Specify your SocialClub Email" << endl;
		string email;
		cin >> email;

		cout << "Specify your SocialClub Profile Name" << endl;
		string nickname;
		cin >> nickname;

		cout << "Specify your SocialClub Password" << endl;
		string password;
		cin >> password;

		env_vars["EMAIL"] = email;
		env_vars["NICKNAME"] = nickname;
		env_vars["PASSWORD"] = password;
		env_vars["PLATFORM"] = "pcros";

		ofstream outFile(".env");

		for (const auto& pair : env_vars) {
			outFile << pair.first << "=" << pair.second << "\n";
		}
	}

	inline bool request_ticket(string email, string nickname, string password, string platform, string& _response)
	{
		map<string, string> map;
		map["ticket"] = "";
		map["email"] = email;
		map["nickname"] = nickname;
		map["password"] = password;
		map["platformName"] = platform;

		string response = run_anonymous("https://gta5-prod.ros.rockstargames.com/gta5/11/gameservices/auth.asmx/CreateTicketSc3", map);

		pugi::xml_document doc;
		if (!doc.load_string(response.c_str())) return false;

		bool successfully = false;

		ofstream outFile("data.bin");

		pugi::xml_node xml_response = doc.child("Response");

		for (pugi::xml_node panel = xml_response.first_child(); panel; panel = panel.next_sibling())
		{
			if (strcmp(panel.name(), "Ticket") == 0)
			{
				TICKET = panel.text().get();
				cout << "Ticket: " << TICKET << endl;
				outFile << "TICKET" << "=" << TICKET << "\n";
				successfully = true;
			}
			else if (strcmp(panel.name(), "SessionKey") == 0)
			{
				SESSION_KEY = panel.text().get();
				cout << "Session Key: " << SESSION_KEY << endl;
				outFile << "SESSION_KEY" << "=" << SESSION_KEY << "\n";
			}
			else if (strcmp(panel.name(), "SessionTicket") == 0)
			{
				SESSION_TICKET = panel.text().get();
				cout << "Session Ticket: " << SESSION_TICKET << endl;
				outFile << "SESSION_TICKET" << "=" << SESSION_TICKET << "\n";
			}
		}

		_response = response;

		return successfully;
	}

	virtual string execute(const vector<string>& args)
	{
		unordered_map<string, string> env_vars;

		ifstream infile("data.bin");

		bool skip;

		if (filesystem::exists("data.bin"))
		{
			string line;
			while (getline(infile, line)) {
				size_t equal_pos = line.find('=');
				if (equal_pos != string::npos) {
					string key = line.substr(0, equal_pos);
					string value = line.substr(equal_pos + 1);

					if (key == "TICKET")
						TICKET = value;
					else if (key == "SESSION_KEY")
						SESSION_KEY = value;
					else if (key == "SESSION_TICKET")
						SESSION_TICKET = value;
				}
			}

			string data = "";
			if (!TICKET.empty())
				data = command::get(hash<string>()("getaccountinfo"))->execute({});

			if (data.empty() || data.contains("Internal Server Error") || data.contains("AuthenticationFailed"))
			{
				cout << "Auth -> Cached session is no longer valid... recreating now." << endl;

				if (filesystem::exists("data.bin"))
				{
					cout << ".env found at " << filesystem::absolute(".env") << endl;
					ifstream infile(".env");
					string line;
					while (getline(infile, line)) {
						size_t equal_pos = line.find('=');
						if (equal_pos != string::npos) {
							string key = line.substr(0, equal_pos);
							string value = line.substr(equal_pos + 1);
							env_vars[key] = value;
						}
					}
					infile.close();

					bool success;
					string response;
					do {
						success = request_ticket(env_vars.at("EMAIL"), env_vars.at("NICKNAME"), env_vars.at("PASSWORD"), env_vars.at("PLATFORM"), response);
						
						if (!success)
						{
							cout << "Auth -> Invalid credentials or rate limited" << endl;
							request_socialclub_data(env_vars);
						}
					} while (!success);

					return response;
				}

			}
			else if (data.contains("Error Code"))
			{
				cout << "Auth -> Received Error: " << data << endl;

				bool success;
				string response;
				do {
					request_socialclub_data(env_vars);
					success = request_ticket(env_vars.at("EMAIL"), env_vars.at("NICKNAME"), env_vars.at("PASSWORD"), env_vars.at("PLATFORM"), response);

					if (!success)
					{
						cout << "Auth -> Invalid credentials or rate limited" << endl;
					}
				} while (!success);

				return response;
			}
			else
			{
				return "Auth -> Using cached session";
			}
		}
		else if (!filesystem::exists(".env") || !filesystem::exists("data.bin"))
		{
			request_socialclub_data(env_vars);

			bool success;
			string response;
			do {
				success = request_ticket(env_vars.at("EMAIL"), env_vars.at("NICKNAME"), env_vars.at("PASSWORD"), env_vars.at("PLATFORM"), response);

				if (!success)
				{
					cout << "Auth -> Invalid credentials or rate limited" << endl;
					request_socialclub_data(env_vars);
				}
			} while (!success);

			return response;
		}

		return "error";
	}
};

CreateTicket g_create_ticket("createticket");