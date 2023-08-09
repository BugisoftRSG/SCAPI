#include "../../../command.hpp"
#include <iostream>

class QueryContent : command
{
	using command::command;

	enum QueryType {
		GetLatestVersionByContentId = 1,
		GetMyBookmarkedContent,
		GetMyContent,
		GetRecentlyCreatedContent,
		GetMyRecentlyPlayedContent,
		GetFriendContent,
		GetTopRatedContent
	};

	QueryType getQueryType(const vector<string>& args) {
		if (args.empty()) {
			cout << "Select one of the following query names:" << endl;
			cout << "(1) GetLatestVersionByContentId" << endl;
			cout << "(2) GetMyBookmarkedContent" << endl;
			cout << "(3) GetMyContent" << endl;
			cout << "(4) GetRecentlyCreatedContent" << endl;
			cout << "(5) GetMyRecentlyPlayedContent" << endl;
			cout << "(6) GetFriendContent" << endl;
			cout << "(7) GetTopRatedContent" << endl;

			int type;
			cin >> type;

			return static_cast<QueryType>(type);
		}
		else {
			return static_cast<QueryType>(stoi(args[0]));
		}
	}

	string getQueryName(QueryType type) {
		switch (type) {
		case GetLatestVersionByContentId:
			return "GetLatestVersionByContentId";
		case GetMyBookmarkedContent:
			return "GetMyBookmarkedContent";
		case GetMyContent:
			return "GetMyContent";
		case GetRecentlyCreatedContent:
			return "GetRecentlyCreatedContent";
		case GetMyRecentlyPlayedContent:
			return "GetMyRecentlyPlayedContent";
		case GetFriendContent:
			return "GetFriendContent";
		case GetTopRatedContent:
			return "GetTopRatedContent";
		}
		return "";
	}

	SERVICE_TYPE get_service_type() { return SERVICE_TYPE::PROD_ROS; }

	virtual string execute(const vector<string>& args)
	{
		string contentId;
		string queryName;
		string count;

		map<string, string> params;
		params["ticket"] = TICKET;
		params["queryName"] = queryName;

		QueryType queryType = getQueryType(args);
		params["queryName"] = getQueryName(queryType);

		if (!args.empty())
			contentId = args[1];
		else if (queryType == QueryType::GetLatestVersionByContentId)
		{
			cout << "Specify the contentId:" << endl;
			cin >> contentId;
		}

		switch (queryType) {
		case GetLatestVersionByContentId:

			params["contentType"] = "gta5mission";
			params["queryParams"] = "{contentids:[\"" + contentId + "\"], lang: ['en', 'fr', 'de', 'it', 'es', 'pt', 'pl', 'ru', 'es-mx'] }";
			params["offset"] = "0";
			params["count"] = "1";
			break;
		case GetMyBookmarkedContent:
		case GetMyContent:
		case GetRecentlyCreatedContent:
		case GetMyRecentlyPlayedContent:
		case GetFriendContent:
		case GetTopRatedContent:
			cout << "Specify the count:" << endl;
			cin >> count;

			params["contentType"] = "gta5mission";
			params["queryParams"] = "{lang:['de','en','fr','it','es','pt','pl','ru','es-mx']}";
			params["offset"] = "0";
			params["count"] = count;
			break;
		}

		return run("http://ugc-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ugc.asmx/QueryContent", params);
	}
};

QueryContent g_query_content("querycontent");