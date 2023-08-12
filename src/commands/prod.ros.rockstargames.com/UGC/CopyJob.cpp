#include "../../../command.hpp"
#include <nlohmann/json.hpp>
#include "../../../ros_crypt.hpp"
#include <cpr/cpr.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

class CopyJob : command
{
	using command::command;

	inline bool GetJobDetails(string_view content_id, string& title, string& description, string& type, vector<string>& tags, string& image)
	{
		cpr::Response response = cpr::Get(
			cpr::Url{ "https://scapi.rockstargames.com/ugc/mission/details" },
			cpr::Header{ {"X-AMC", "true" }, { "X-Requested-With", "XMLHttpRequest"} },
			cpr::Parameters{ {"title", "gtav"}, {"contentId", content_id.data()} });

		nlohmann::json job_details = nlohmann::json::parse(string{ response.text });

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

	inline bool GetJobMetaData(string_view content_id, string& result, string& lang)
	{
		string data = command::get(hash<string>()("querycontent"))->execute({ "1", content_id.data()});
		pugi::xml_document doc;
		int f0 = 0, f1 = 0;

		if (doc.load_string(data.c_str()))
		{
			auto path = doc.select_node("//Response/Result/r/m");
			auto node = path.node();

			f1 = node.attribute("f1").as_int();
			f0 = node.attribute("f0").as_int();
			lang = node.attribute("l").as_string();
		}

		cpr::Response response = cpr::Get(cpr::Url{ format("https://prod.cloud.rockstargames.com/ugc/gta5mission/{}/{}_{}_{}.json", content_id, f1 < 0 ? 0 : f1, f0 < 0 ? 0 : f0, lang.empty() ? "en" : lang) });

		result = response.text;

		return response.status_code == 200;
	}

	vector<unsigned char> HexToBytes(const string& hex) {
		vector<unsigned char> bytes;

		for (unsigned int i = 0; i < hex.length(); i += 2) {
			string byteString = hex.substr(i, 2);
			unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
			bytes.push_back(byte);
		}

		return bytes;
	}

	vector<unsigned char> get_data_len(size_t data_len, bool image) // should be 107684
	{
		stringstream ss;
		ss << hex << data_len;
		string data_len_hex = ss.str();


		if (data_len_hex.length() % 2 != 0) {
			data_len_hex = "0" + data_len_hex;
		}

		vector<unsigned char> data_len_bytes = HexToBytes(data_len_hex); // should be \x01\xa4\xa4

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
			data_len_bytes.push_back(2);
		else 
			data_len_bytes.push_back(0);

		return data_len_bytes;
	}

	string replaceAll(string const& original, string const& from, string const& to)
	{
		string results;
		string::const_iterator end = original.end();
		string::const_iterator current = original.begin();
		string::const_iterator next = search(current, end, from.begin(), from.end());
		while (next != end) {
			results.append(current, next);
			results.append(to);
			current = next + from.size();
			next = search(current, end, from.begin(), from.end());
		}
		results.append(current, next);
		return results;
	}

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the content id:" << endl;
		string content_id;
		cin >> content_id;

		string title, description, type, image;
		vector<string> tags;
		string metadata, lang;

		if (GetJobDetails(content_id, title, description, type, tags, image))
		{
			if (!GetJobMetaData(content_id, metadata, lang))
			{
				return "Couldn't download job metadata";
			}
		}

		map<string, string> map;

		ofstream of("2_0.jpg", ios::binary);
		cpr::Response r = cpr::Download(of, cpr::Url{ image });
		ifstream input("2_0.jpg", ios_base::binary);
		vector<unsigned char> image_bytes((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));

		nlohmann::json meta_json = nlohmann::json::parse(metadata);
		
		vector<unsigned char> data_len = get_data_len(meta_json.dump().size(), false);
		vector<unsigned char> image_data_len = get_data_len(image_bytes.size(), true);

		nlohmann::json datajson;
		datajson["meta"] = meta_json["meta"];
		datajson["mission"] = meta_json["mission"];
		datajson["version"] = 2;

		auto dataJsonDump = datajson.dump();

		string temp = format("ticket={}&contentType=gta5mission&paramsJson=", m_ros->url_encode(TICKET));

		std::string tagsSep = std::accumulate(
			std::begin(tags),
			std::end(tags),
			std::string(),
			[](const std::string& accumulator, const std::string& element) {
				return accumulator.empty() ? element : accumulator + "," + element;
			}
		);

		nlohmann::json obj;
		obj["ContentName"] = title;
		obj["DataJson"] = m_ros->ex_url_encode(replaceAll(datajson.dump(), "\"", "\\\""));
		obj["Description"] = description;
		obj["Publish"] = "true";
		obj["Language"] = lang.empty() ? "en" : lang;
		obj["TagCsv"] = tagsSep;

		temp += obj.dump();
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

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/CreateContent", temp);
	}
};

CopyJob g_copy_job("copyjob");