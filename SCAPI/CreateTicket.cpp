#include "command.hpp"
#include "pugixml.hpp"
#include <iostream>

class CreateTicket : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;
		map["ticket"] = "";
		map["email"] = args[0];
		map["nickname"] = args[1];
		map["password"] = args[2];
		map["platformName"] = args[3];

		std::string response = run_create_ticket("http://auth-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/auth.asmx/CreateTicketSc3", 42, map);
		pugi::xml_document doc;
		if (!doc.load_string(response.c_str())) return response;
		
		pugi::xml_node xml_response = doc.child("Response");

		for (pugi::xml_node panel = xml_response.first_child(); panel; panel = panel.next_sibling())
		{
			if (strcmp(panel.name(), "Ticket") == 0)
			{
				TICKET = panel.text().get();
				std::cout << "Ticket: " << TICKET << std::endl;
			}
			else if (strcmp(panel.name(), "SessionKey") == 0)
			{
				SESSION_KEY = panel.text().get();
				std::cout << "Session Key: " << SESSION_KEY << std::endl;
			}
			else if (strcmp(panel.name(), "SessionTicket") == 0)
			{
				SESSION_TICKET = panel.text().get();
				std::cout << "Session Ticket: " << SESSION_TICKET << std::endl;
			}
		}

		return response;
	}
};

CreateTicket g_create_ticket("createticket");