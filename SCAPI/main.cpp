#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <format>
#include "command.hpp"
#include "ros_crypt.hpp"

using namespace std;

int main()
{
	string endpoint;

	HWND hwnd = FindWindowA(NULL, "Grand Theft Auto V");
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid);

	char ticket[208];
	ReadProcessMemory(phandle, (void*)(0x7FF7DC928AD0), &ticket, sizeof(ticket), 0);

	char session_ticket[88];
	ReadProcessMemory(phandle, (void*)(0x7FF7DC928AD0 + 512), &session_ticket, sizeof(ticket), 0);

	unsigned char session_key[16];
	ReadProcessMemory(phandle, (void*)(0x7FF7DC928AD0 + 1528), &session_key, 16, 0);

	TICKET = ticket;
	SESSION_TICKET = session_ticket;
	SESSION_KEY = std::string(a_base64_encode(session_key, sizeof(session_key)), 24);

	if (!TICKET.empty())
	{
		cout << "Enter the endpoint:" << endl;
		cin >> ws >> endpoint;
	}
	else
	{
		endpoint = "createticket";
	}

	while (true)
	{

		if (endpoint == "decrypt")
		{
			std::ifstream infile(R"(C:\Users\Bugisoft\Downloads\GTA_Reverse\request_content.txt)", std::ios_base::binary);

			std::vector<char> bytes((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

			std::cout << urlDecode(DecryptROSData(&bytes[0], bytes.size(), SESSION_KEY)) << endl;
		}			

		static command* command = command::get(std::hash<std::string>()(endpoint));

		if (command) {
			if (command->get_name() == "querycontent")
			{
				cout << "Specify the content id:" << endl;
				string content_id;
				cin >> content_id;

				cout << command->execute({ content_id }) << endl;
			}
			else if (command->get_name() == "sendinvite")
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

				cout << command->execute({ sender_rid, sender_name, target_rid, session_info }) << endl;
			}
			else if (command->get_name() == "sendemail")
			{
				cout << "Specify the target rid:" << endl;
				string target_rid;
				cin >> target_rid;

				cout << command->execute({ target_rid }) << endl;
			}
			else if (command->get_name() == "sendadminmsg")
			{
				cout << "Specify the target rid:" << endl;
				string target_rid;
				cin >> target_rid;

				cout << command->execute({ target_rid }) << endl;
			}
			else if (command->get_name() == "createticket")
			{
				cout << "Specify the email:" << endl;
				string email;
				cin >> email;

				cout << "Specify the nickname:" << endl;
				string nickname;
				cin >> nickname;

				cout << "Specify the password:" << endl;
				string password;
				cin >> password;

				cout << "Specify the platformName:" << endl;
				string platformName;
				cin >> platformName;

				command->execute({ email, nickname, password, platformName });
			}
			else if (command->get_name() == "getlocationinfoFromip")
			{
				cout << "Specify the ip:" << endl;
				string target_rid;
				cin >> target_rid;

				cout << command->execute({ target_rid }) << endl;

			}
			else if (command->get_name() == "matchmaking")
			{
				cout << command->execute({  }) << endl;
			}

			cout << "Enter the endpoint:" << endl;
			cin >> ws >> endpoint;
			command = command::get(std::hash<std::string>()(endpoint));
		}
		endpoint = "";
	}
}