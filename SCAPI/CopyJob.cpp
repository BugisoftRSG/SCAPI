#include "command.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "HTTPRequest.hpp"
#include "json.hpp"
#include "ros_crypt.hpp"

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

	std::vector<unsigned char> get_data_len(std::string metadata)
	{
		std::vector<unsigned char> bytes(metadata.begin(), metadata.end());

		int data_len = bytes.size();

		std::stringstream ss;
		ss << std::hex << data_len;
		std::string data_len_hex = ss.str();

		if (data_len_hex.length() % 2 != 0) {
			data_len_hex = "0" + data_len_hex;
		}

		std::cout << data_len_hex << std::endl;

		data_len_hex = "01a4a4"; // WRONG FIX THIS

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
		data_len_bytes.push_back(0);

		return data_len_bytes;
	}

	std::vector<unsigned char> get_image_data_len()
	{
		std::ifstream infile(R"(C:\Users\Bugisoft\Downloads\2_0 (1).jpg)", std::ios_base::binary);

		std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

		int data_len = bytes.size();

		std::stringstream ss;
		ss << std::hex << data_len;
		std::string data_len_hex = ss.str();

		if (data_len_hex.length() % 2 != 0) {
			data_len_hex = "0" + data_len_hex;
		}

		std::vector<unsigned char> data_len_bytes = HexToBytes(data_len_hex); // should be \xdd@

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

		// image_len = \x00\x00\xdd@\x00\x00\x00\x02
		//get_image_data_len(); // \x00\x01\xa4\xa4\x00\x00\x00\x00

		std::map<std::string, std::string> map;

		http::Request http{ "http://prod.cloud.rockstargames.com/ugc/gta5mission/1nMz02Hpf0KM2O1ZT3Exeg/0_0_en.json" };
		const auto request = http.send("GET");

		std::string data = std::string{ request.body.begin(), request.body.end() };

		// \x00\x01\x7c\xc1\x00\x00\x00\x00 should be \x00\x01\xa4\xa4\x00\x00\x00\x00 Okay
		std::vector<unsigned char> data_len = get_data_len(data);
		for (auto val : data_len) printf("\\x%.2x", val);
		std::cout << std::endl;

		// \x00\x00\xdd\x40\x00\x00\x00\x02 should be \x00\x00\xdd@\x00\x00\x00\x02
		std::vector<unsigned char> image_data_len = get_image_data_len();


		nlohmann::json meta_json = nlohmann::json::parse(data);

		nlohmann::json datajson;
		datajson["meta"] = meta_json["meta"];
		datajson["mission"] = meta_json["mission"];
		datajson["version"] = 2;

		std::string data_1(data_len.begin(), data_len.end());
		std::string image_data_1(image_data_len.begin(), image_data_len.end());

		std::ifstream infile(R"(C:\Users\Bugisoft\Downloads\2_0 (1).jpg)", std::ios_base::binary);

		std::vector<char> image_bytes((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

		std::string temp = "ticket=rxC5MkFxCp5tTCWktW33dt61awAlr0B6Vb4l4%2BJnNuF1iWIq21QZJEz59zMWZH6JgeONm%2BVBOabsXNJDxNZU9N8cguIvZEWRdkbRgmOJBKzxJ0vKR3XPpnUXcKfiT8H472BLLmQyMjaUmCA7U9EYsBMC39zsbbiFMCONkW1hah4YB8eyy50vO2JGTzIE6NFanLI64smw%2Bw%3D%3D&contentType=gta5mission&paramsJson=";
		std::cout << temp << std::endl;

		std::ifstream t("meta.txt");
		std::stringstream hm;
		hm << t.rdbuf();
		temp += url_encode(std::format(R"({{"ContentName":"{}","DataJson":"{}","Description":"{}","Publish":true,"Language":"{}","TagCsv":"{}"}})", "X Test", hm.str(), "X Test", "en", ""));
		temp += "&data=";
		
		for (auto& i : data_len) {
			temp += i;
		}
		std::ifstream td("data_json.txt");
		std::stringstream asd;
		asd << td.rdbuf();
		temp += asd.str();
		for (auto& i : image_data_len) {
			temp += i;
		}
		for (auto& i : image_bytes) {
			temp += i;
		}

		std::ofstream myfile;
		myfile.open("data.txt");
		myfile << map["data"];
		myfile.close();

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CreateContent", 42, temp);
	}
};

CopyJob g_copy_job("copyjob");