#pragma once
#include <cstdint>

class AccountInfo
{
public:
	uint64_t m_rockstar_id;
	int m_age;
	string m_avatar_url;
	string m_country_code;
	string m_date_of_birth;
	string m_email;
	bool m_is_approx_dob;
	string m_language_code;
	string m_nickname;
	string m_last_portal_login_date;
};