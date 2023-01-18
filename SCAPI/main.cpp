#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <format>
#include <Windows.h>
#include <tlhelp32.h>
#include "command.hpp"
#include "json.hpp"
#include "cpr/cpr.h"
#include <pugixml.hpp>

using namespace std;

const std::vector<std::string> languages = { "unset", "en", "fr", "de", "it", "es", "pt", "pl", "ru", "es-mx" };

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };
static char* decoding_table = NULL;
static int mod_table[] = { 0, 2, 1 };

using namespace std;

inline void base64_cleanup()
{
	free(decoding_table);
}

inline void build_decoding_table()
{
	atexit(base64_cleanup);

	decoding_table = (char*)malloc(256);

	for (int i = 0; i < 64; i++)
		decoding_table[(unsigned char)encoding_table[i]] = i;
}

inline char* a_base64_encode(const unsigned char* data, size_t input_length)
{
	size_t output_length;

	output_length = 4 * ((input_length + 2) / 3);

	char* encoded_data = (char*)malloc(output_length);
	if (encoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;)
	{

		uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[output_length - 1 - i] = '=';

	return encoded_data;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

bool GetJobDetails(string_view content_id, std::string& title, std::string& description, std::string& type, std::vector<std::string>& tags, std::string& image)
{
	cpr::Response response = cpr::Get(
		cpr::Url{ "https://scapi.rockstargames.com/ugc/mission/details" },
		cpr::Header{ {"X-AMC", "true" }, { "X-Requested-With", "XMLHttpRequest"} },
		cpr::Parameters{ {"title", "gtav"}, {"contentId", content_id.data()} });

	nlohmann::json job_details = nlohmann::json::parse(std::string{ response.text });

	if (job_details["content"].is_object())
	{
		nlohmann::json content = job_details["content"];

		title = content["name"];
		description = content["desc"];
		type = content["type"];
		tags = content["userTags"];
		image = content["imgSrc"];

		return true;
	}

	return false;
}

bool GetJobMetaData(string_view content_id, string& result)
{
	std::string data = command::get(std::hash<std::string>()("querycontent"))->execute({ content_id.data() });
	pugi::xml_document doc;
	int f0, f1;
	std::string lang;

	if (doc.load_string(data.c_str())) 
	{
		auto path = doc.select_node("//Response/Result/r/m");
		auto node = path.node();

		f1 = node.attribute("f1").as_int();
		f0 = node.attribute("f0").as_int();
		lang = node.attribute("l").as_string();
	}

	cpr::Response response = cpr::Get(cpr::Url{ std::format("https://prod.cloud.rockstargames.com/ugc/gta5mission/{}/{}_{}_{}.json", content_id, f1 < 0 ? 0 : f1, f0 < 0 ? 0 : f0, lang.empty() ? "en" : lang)});

	result = response.text;

	return response.status_code == 200;
}

int main()
{
	string endpoint;

	HWND hwnd = FindWindowA(NULL, "Grand Theft Auto V");
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	DWORD64 base_address = GetModuleBaseAddress(pid, L"GTA5.exe");
	cout << (base_address + 0x2CA8AD0) + 0x200 << endl;

	char ticket[208]{};
	ReadProcessMemory(phandle, (void*)(base_address + 0x2CA8AD0), &ticket, 208, 0);

	char session_ticket[88]{};
	ReadProcessMemory(phandle, (void*)(base_address + 0x2CA8AD0 + 0x200), &session_ticket, 88, 0);

	unsigned char session_key[16]{};
	ReadProcessMemory(phandle, (void*)(base_address + 0x2CA8AD0 + 0x5F8), &session_key, 16, 0);

	TICKET = ticket;
	SESSION_TICKET = std::string(session_ticket, 88);
	SESSION_KEY = std::string(a_base64_encode(session_key, sizeof(session_key)), 24);

	cout << "Ticket " << TICKET << endl;
	cout << "Session Ticket " << SESSION_TICKET << endl;
	cout << "Session Key " << SESSION_KEY << endl;

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
		static command* command = command::get(std::hash<std::string>()(endpoint));

		if (command) {
			if (command->get_name() == "decrypt")
			{
				cout << command->execute({ }) << endl;
			}
			else if (command->get_name() == "querycontent")
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
			else if (command->get_name() == "getlocationinfofromip")
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
			else if (command->get_name() == "copylocalcontent")
			{
				cout << command->execute({  }) << endl;
			}
			else if (command->get_name() == "copyjob")
			{
				cout << "Specify the content id:" << endl;
				string content_id;
				cin >> content_id;

				std::string title, description, type, image;
				vector<std::string> tags;
				if (GetJobDetails(content_id, title, description, type, tags, image))
				{
					string metadata;

					if (GetJobMetaData(content_id, metadata))
					{
						cout << command->execute({ metadata, image, title, description }) << endl;
					}
				}
			}

			cout << "Enter the endpoint:" << endl;
			cin >> ws >> endpoint;
			command = command::get(std::hash<std::string>()(endpoint));
		}
		endpoint = "";
	}
}