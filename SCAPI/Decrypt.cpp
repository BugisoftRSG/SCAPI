#include "command.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ros_crypt.hpp"

class Decrypt : command
{
	using command::command;

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::ifstream infile(R"(C:\Users\Bugisoft\Desktop\Dev\GTA Reverse\request_content.txt)", std::ios_base::binary);

		std::vector<char> bytes((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

		ofstream myfile;
		myfile.open("nonencoded.txt");
		myfile << DecryptROSData(&bytes[0], bytes.size(), SESSION_KEY);
		myfile.close();

		return "";
		//return urlDecode(DecryptROSData(&bytes[0], bytes.size(), SESSION_KEY));
	}
};

Decrypt g_decrypt("decrypt");