#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <format>
#include "command.hpp"
#include <nlohmann/json.hpp>
#include "cpr/cpr.h"
#include <pugixml.hpp>
#include "AccountInfo.hpp"
#include <botan_all.h>

#ifdef _WIN32
	#include <Windows.h>
	#include <tlhelp32.h>
#endif

using namespace std;

#ifdef _WIN32
	uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
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
					if (!strcmp(modEntry.szModule, modName))
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
#endif

AccountInfo GetAccountInfo()
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

/*
*  <Services Count="12">
    <S ep="/Accounts.svc/" h="accounts-prod.ros.rockstargames.com" />
    <S ep="/Feed.asmx/" h="feed-gta5-prod.ros.rockstargames.com" />
    <S ep="/Telemetry.asmx/SubmitCompressed" h="prod.telemetry.ros.rockstargames.com" />
    <S ep="/Telemetry.asmx/SubmitRealTime" h="prod.telemetry.ros.rockstargames.com" />
    <S ep="conductor" h="conductor-prod.ros.rockstargames.com" />
    <S ep="/ProfileStats.asmx/" h="ps-gta5-prod.ros.rockstargames.com" />
    <S ep="/matchmaking.asmx/" h="mm-gta5-prod.ros.rockstargames.com" />
    <S ep="/ugc.asmx/" h="ugc-gta5-prod.ros.rockstargames.com" />
    <S ep="/Presence.asmx/" h="prs-gta5-prod.ros.rockstargames.com" />
    <S ep="/Inbox.asmx/" h="inbox-gta5-prod.ros.rockstargames.com" />
    <S ep="/Clans.asmx/" h="crews-gta5-prod.ros.rockstargames.com" />
    <S ep="/cloudservices/members//GTA5/saves/mpstats" h="cs-gta5-prod.ros.rockstargames.com" />
  </Services>
*/

int main()
{
	string endpoint;

#ifdef _WIN64
	SetConsoleOutputCP(CP_UTF8);

	HWND hwnd = FindWindowA(NULL, "Grand Theft Auto V");
	if (hwnd)
	{
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		uintptr_t base_address = GetModuleBaseAddress(pid, "GTA5.exe");

		char ticket[208]{};
		ReadProcessMemory(phandle, (void*)(base_address + 0x2E7B300), &ticket, 208, 0);

		char session_ticket[88]{};
		ReadProcessMemory(phandle, (void*)(base_address + 0x2E7B300 + 0x200), &session_ticket, 88, 0);

		unsigned char session_key[16]{};
		ReadProcessMemory(phandle, (void*)(base_address + 0x2E7B300 + 0x608), &session_key, 16, 0);

		TICKET = ticket;
		SESSION_TICKET = string(session_ticket, 88);
		SESSION_KEY = Botan::base64_encode(reinterpret_cast<const Botan::byte*>(session_key), sizeof(session_key));
	}
#endif

	cout << "Help:" << endl;
	for (auto& it : g_commands) {
		cout << it.second->get_name() << endl;
	}

	if (!TICKET.empty())
	{
		cout << "Ticket " << TICKET << endl;
		cout << "Session Ticket " << SESSION_TICKET << endl;
		cout << "Session Key " << SESSION_KEY << endl;

		cout << "Enter the endpoint:" << endl;
		cin >> ws >> endpoint;
	}
	else
	{
		string response = command::get(hash<string>()("createticket"))->execute({});

		if (TICKET.length() > 20)
		{
			cout << "Ticket " << TICKET << endl;
			cout << "Session Ticket " << SESSION_TICKET << endl;
			cout << "Session Key " << SESSION_KEY << endl;
		}
	}

	while (true)
	{
		transform(endpoint.begin(), endpoint.end(), endpoint.begin(), ::tolower);
		static command* command = command::get(hash<string>()(endpoint));

		if (command) {
			cout << command->execute({ }) << endl;
		}
		cout << "Enter the endpoint:" << endl;
		cin >> ws >> endpoint;
		command = command::get(hash<string>()(endpoint));
	}
}