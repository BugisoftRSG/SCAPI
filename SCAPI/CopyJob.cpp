#include "command.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.hpp"
#include "ros_crypt.hpp"
#include <cpr/cpr.h>

class CopyJob : command
{
	using command::command;

	std::vector<unsigned char> HexToBytes(const std::string& hex) {
		std::vector<unsigned char> bytes;

		for (unsigned int i = 0; i < hex.length(); i += 2) {
			std::string byteString = hex.substr(i, 2);
			unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
			bytes.push_back(byte);
		}

		return bytes;
	}

	std::vector<unsigned char> get_data_len(size_t data_len, bool image) // should be 107684
	{
		std::stringstream ss;
		ss << std::hex << data_len;
		std::string data_len_hex = ss.str();

		if (data_len_hex.length() % 2 != 0) {
			data_len_hex = "0" + data_len_hex;
		}

		//data_len_hex = "01a4a4"; // WRONG FIX THIS

		std::vector<unsigned char> data_len_bytes = HexToBytes(data_len_hex); // should be \x01\xa4\xa4

		switch (data_len_bytes.size()) {
		case 1:
			data_len_bytes.insert(data_len_bytes.begin(), 3, 0);
			break;
		case 2:
			data_len_bytes.insert(data_len_bytes.begin(), 2, 0);
			break;
		case 3:
			data_len_bytes.insert(data_len_bytes.begin(), 1, 0);
			break;
		default:
			break;
		}

		data_len_bytes.insert(data_len_bytes.end(), 3, 0);
		if (image) 
			data_len_bytes.push_back(0);
		else 
			data_len_bytes.push_back(2);

		return data_len_bytes;
	}

	std::string
		replaceAll(std::string const& original, std::string const& from, std::string const& to)
	{
		std::string results;
		std::string::const_iterator end = original.end();
		std::string::const_iterator current = original.begin();
		std::string::const_iterator next = std::search(current, end, from.begin(), from.end());
		while (next != end) {
			results.append(current, next);
			results.append(to);
			current = next + from.size();
			next = std::search(current, end, from.begin(), from.end());
		}
		results.append(current, next);
		return results;
	}

	virtual std::string execute(const std::vector<std::string>& args)
	{
		std::map<std::string, std::string> map;

		//std::ofstream of("2_0.jpg", std::ios::binary);
		//cpr::Response r = cpr::Download(of, cpr::Url{ args[1] });

		std::ifstream input(R"(C:\Users\Bugisoft\Downloads\2_0.jpg)");
		std::vector<unsigned char> image_bytes((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

		std::string data = args[0];
		nlohmann::json meta_json = nlohmann::json::parse(data);

		std::vector<unsigned char> data_len = get_data_len(meta_json.dump().length(), false);
		std::vector<unsigned char> image_data_len = get_data_len(image_bytes.size(), true);

		nlohmann::json datajson;
		datajson["meta"] = meta_json["meta"];
		datajson["mission"] = meta_json["mission"];
		datajson["version"] = 2;

		std::string temp = std::format("ticket={}&contentType=gta5mission&paramsJson=", url_encode(TICKET));

		temp += ex_url_encode(std::format(R"({{"ContentName":"{}","DataJson":"{}","Description":"{}","Publish":true,"Language":"{}","TagCsv":"{}"}})", args[2], replaceAll(replaceAll(replaceAll(datajson.dump(), ",", ",+"), "\"", "\\\""), ":", ":+"), args[3], "de", ""));
		temp += "&data=";
		
		for (auto& i : data_len) 
		{
			temp += i;
		}

		temp += meta_json.dump();

		for (auto& i : image_data_len) 
		{
			temp += i;
		}
		for (auto& i : image_bytes)
		{
			temp += i;
		}

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CreateContent", 42, temp);
	}
};

CopyJob g_copy_job("copyjob");