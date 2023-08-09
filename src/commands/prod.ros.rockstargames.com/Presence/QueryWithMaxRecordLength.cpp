#include "../../../command.hpp"
#include <format>
#include <iostream>

class QueryWithMaxRecordLength : command
{
	using command::command;

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		cout << "Specify the target rid:" << endl;
		string target_rid;
		cin >> target_rid;

		map<string, string> map;
		map["ticket"] = TICKET;
		map["queryName"] = "SessionByGamerHandle";
		map["paramNameValueCsv"] = format(R"(@ghandle,"SC+{}")", target_rid);
		map["offset"] = "0";
		map["count"] = "20";
		map["maxQueryBufferSize"] = "7680";

		return run("http://prs-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/Presence.asmx/QueryWithMaxRecordLength", map);
	}
};

QueryWithMaxRecordLength query_with_max_record_length("querywithmaxrecordlength");