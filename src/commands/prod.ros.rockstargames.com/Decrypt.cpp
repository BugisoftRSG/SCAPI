#include "../../command.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../ros_crypt.hpp"

class Decrypt : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		ifstream infile(R"(C:\Users\Bugisoft\Downloads\GTA_Reverse\request_content.txt)", ios_base::binary);

		vector<char> bytes((istreambuf_iterator<char>(infile)), (istreambuf_iterator<char>()));

		return m_ros->DecryptROSData(&bytes[0], bytes.size(), SESSION_KEY);
	}
};

Decrypt g_decrypt("decrypt");