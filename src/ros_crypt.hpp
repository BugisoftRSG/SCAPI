#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include "botan_all.h"

using namespace std;

class ROSCryptoState
{
private:
    Botan::StreamCipher* m_rc4;
    uint8_t m_rc4Key[32];
    uint8_t m_xorKey[16];
    uint8_t m_hashKey[16];

public:
    ROSCryptoState(const string& platformKey);
    ~ROSCryptoState();

    const uint8_t* GetXorKey() const;
    const uint8_t* GetHashKey() const;
};

class ROSCrypt
{
public:
    constexpr ROSCrypt(const bool launcher) : m_launcher(launcher)
    {
		constexpr const char* launcherPlatformKey = "C6fU6TQTPgUVmy3KIB5g8ElA7DrenVpGogSZjsh+lqJaQHqv4Azoctd/v4trfX6cBjbEitUKBG/hRINF4AhQqcg=";
		constexpr const char* platformKey = "C4pWJwWIKGUxcHd69eGl2AOwH2zrmzZAoQeHfQFcMelybd32QFw9s10px6k0o75XZeB5YsI9Q9TdeuRgdbvKsxc=";

        m_cryptoState = make_unique<ROSCryptoState>(launcher ? launcherPlatformKey : platformKey);
    }

    string EncryptROSData(const string& input, const string& sessionKey = "");
    string DecryptROSData(const char* data, size_t size, const string& sessionKey);

    string url_encode(const string& value);
    string ex_url_encode(const string& value);

    string BuildPostString(const map<string, string>& fields);

	string GetROSVersionString();

    template<typename TAlloc>
	auto HeadersHmac(const vector<uint8_t, TAlloc>& challenge, const char* method, const string& path, const string& sessionKey, const string& sessionTicket);

private:
    unique_ptr<ROSCryptoState> m_cryptoState;
    bool m_launcher;
};

// TODO compiler error when in .cpp?
template<typename TAlloc>
inline auto ROSCrypt::HeadersHmac(const vector<uint8_t, TAlloc>& challenge, const char* method, const string& path, const string& sessionKey, const string& sessionTicket)
{
	auto hmac = unique_ptr<Botan::MessageAuthenticationCode>(Botan::MessageAuthenticationCode::create("HMAC(SHA1)")->clone());

	auto cryptoState = m_cryptoState.get();

	// set the key
	uint8_t hmacKey[16];

	// xor the RC4 key with the platform key (and optionally the session key)
	auto rc4Xor = Botan::base64_decode(sessionKey);

	for (int i = 0; i < sizeof(hmacKey); i++)
	{
		hmacKey[i] = rc4Xor[i] ^ cryptoState->GetXorKey()[i];
	}

	hmac->set_key(hmacKey, sizeof(hmacKey));

	// method
	hmac->update(method);
	hmac->update(0);

	// path
	hmac->update(path);
	hmac->update(0);

	// ros-SecurityFlags
	hmac->update("239");
	hmac->update(0);

	// ros-SessionTicket
	hmac->update(sessionTicket);
	hmac->update(0);

	// ros-Challenge
	hmac->update(Botan::base64_encode(challenge));
	hmac->update(0);

	// platform hash key
	hmac->update(cryptoState->GetHashKey(), 16);

	// set the request header
	auto hmacValue = hmac->final();

	return hmacValue;
}
