#pragma once
#include <string>
#include <unordered_map>
#include <map>

inline std::string TICKET = "";
inline std::string SESSION_TICKET = "";
inline std::string SESSION_KEY = "";

class command
{
protected:
	std::string m_name;
	static std::string run(const std::string& url, int offset, std::map<std::string, std::string> map);
	static std::string run_create_ticket(const std::string& url, int offset, std::map<std::string, std::string> map);
public:
	command(const std::string& name);
	inline const std::string& get_name() { return m_name; }
	static command* get(int command);
	virtual std::string execute(const std::vector<std::string>& args) = 0;
};

inline std::unordered_map<int, command*> g_commands;