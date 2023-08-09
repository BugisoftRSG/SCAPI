#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <iostream>
#include <pugixml.hpp>
#include "ros_crypt.hpp"

using namespace std;

inline string TICKET;
inline string SESSION_TICKET;
inline string SESSION_KEY;

enum SERVICE_TYPE
{
	UNKNOWN,
	PROD_ROS,
	PROD_ROS_LAUNCHER,
	PROD_SCAPI_AMC,
	PROD_CLOUD
};

class command
{
protected:
	string m_name;

	string run_raw(const string url, const string queryString, bool launcher = false);
	string run(const string url, map<string, string> map);
	string run(const string url, string params);
	string run_anonymous(const string& url, map<string, string> map);
public:

	command(const string& name);
	inline const string& get_name() { return m_name; }
	static command* get(int command);
	virtual string execute(const vector<string>& args) = 0;
	virtual SERVICE_TYPE get_service_type() = 0;

	inline string get_category()
	{
		switch (get_service_type())
		{
		case PROD_ROS:
			return "prod.ros";
			break;
		case PROD_ROS_LAUNCHER:
			return "prod.ros (launcher)";
			break;
		case PROD_SCAPI_AMC:
			return "SCAPI";
			break;
		case PROD_CLOUD:
			return "cloud";
			break;
		default:
			return "unknown";
			break;
		}
	}

	static ROSCrypt* m_launcher_ros;
	static ROSCrypt* m_ros;
};

inline unordered_map<int, command*> g_commands;