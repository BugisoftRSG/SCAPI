#include "../../../command.hpp"
#include <format>

class SendEmail : command
{
	using command::command;

	class rlGamerHandle
	{
	public:
		uint64_t m_rockstar_id; //0x0000
		uint8_t m_platform = 3; //0x0008
		uint8_t unk_0009; //0x0009
	}; //Size: 0x0010
	static_assert(sizeof(rlGamerHandle) == 0x10);

	string rlGamerHandleToBase64(const rlGamerHandle& handle)
	{
		const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		vector<uint8_t> bytes(sizeof(rlGamerHandle));
		memcpy(bytes.data(), &handle, sizeof(rlGamerHandle));

		string result;
		size_t paddingCount = (3 - bytes.size() % 3) % 3;

		for (size_t i = 0; i < bytes.size(); i += 3)
		{
			uint32_t value = (bytes[i] << 16) | ((i + 1 < bytes.size() ? bytes[i + 1] : 0) << 8) | ((i + 2 < bytes.size() ? bytes[i + 2] : 0));
			result += base64Chars[(value >> 18) & 0x3F];
			result += base64Chars[(value >> 12) & 0x3F];
			result += (i + 1 < bytes.size() || paddingCount < 2) ? base64Chars[(value >> 6) & 0x3F] : '=';
			result += (i + 2 < bytes.size() || paddingCount < 1) ? base64Chars[value & 0x3F] : '=';
		}

		return result;
	}

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the sender rid:" << endl;
		string sender_rid;
		cin >> sender_rid;

		cout << "Specify the target rid:" << endl;
		string target_rid;
		cin >> target_rid;

		cin.ignore(1000000, '\n');

		cout << "Specify the subject:" << endl;
		string subject;
		getline(cin, subject);

		cin.ignore(1000000, '\n');

		cout << "Specify the content:" << endl;
		string content;
		getline(cin, content);

		rlGamerHandle handle = rlGamerHandle(stoull(sender_rid));

		map<string, string> map;
		map["ticket"] = TICKET;
		map["userIds"] = target_rid;
		map["message"] = format(R"({{"email":{{"gh":"{}","sb":"{}","cn":"{}"}}}})", rlGamerHandleToBase64(handle), subject, content);
		map["tagsCsv"] = "gta5email"; // gta5marketing
		map["ttlSeconds"] = "2592000"; // 1800

		return run("http://inbox-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Inbox.asmx/PostMessageToRecipients", map);
	}
};

SendEmail g_send_email("sendemail");