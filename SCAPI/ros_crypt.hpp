#pragma once
#include <botan/botan.h>
#include <botan/base64.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib")
#include "HTTPRequest.hpp"
#include <iostream>
#include <format>
#include <zlib.h>

// C4pWJwWIKGUxcHd69eGl2AOwH2zrmzZAoQeHfQFcMelybd32QFw9s10px6k0o75XZeB5YsI9Q9TdeuRgdbvKsxc= PC
// C6i91R73oCD3qt1kUh0UIkDTu3Su5Qa7/r74q5ohUj1UxX/yQz7qB8a4y2TXfCMxqJo31tOPuZJMwG3jupDl7rs= PS4
// C6fU6TQTPgUVmy3KIB5g8ElA7DrenVpGogSZjsh+lqJaQHqv4Azoctd/v4trfX6cBjbEitUKBG/hRINF4AhQqcg= LAUNCHER
#define ROS_PLATFORM_KEY "C4pWJwWIKGUxcHd69eGl2AOwH2zrmzZAoQeHfQFcMelybd32QFw9s10px6k0o75XZeB5YsI9Q9TdeuRgdbvKsxc="


using namespace std;

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };
static char* decoding_table = NULL;
static int mod_table[] = { 0, 2, 1 };

using namespace std;

inline void base64_cleanup()
{
	free(decoding_table);
}

inline void build_decoding_table()
{
	atexit(base64_cleanup);

	decoding_table = (char*)malloc(256);

	for (int i = 0; i < 64; i++)
		decoding_table[(unsigned char)encoding_table[i]] = i;
}

inline char* a_base64_encode(const unsigned char* data, size_t input_length)
{
	size_t output_length;

	output_length = 4 * ((input_length + 2) / 3);

	char* encoded_data = (char*)malloc(output_length);
	if (encoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;)
	{

		uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[output_length - 1 - i] = '=';

	return encoded_data;
}

inline unsigned char* a_base64_decode(const char* data, size_t input_length)
{
	size_t output_length;

	if (decoding_table == NULL) build_decoding_table();

	if (input_length % 4 != 0) return NULL;

	output_length = input_length / 4 * 3;
	if (data[input_length - 1] == '=') (output_length)--;
	if (data[input_length - 2] == '=') (output_length)--;

	unsigned char* decoded_data = (unsigned char*)malloc(output_length);
	if (decoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;)
	{

		uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

		uint32_t triple = (sextet_a << 3 * 6)
			+ (sextet_b << 2 * 6)
			+ (sextet_c << 1 * 6)
			+ (sextet_d << 0 * 6);

		if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}

	return decoded_data;
}

class ROSCryptoState
{
private:
	Botan::StreamCipher* m_rc4;

	uint8_t m_rc4Key[32];
	uint8_t m_xorKey[16];
	uint8_t m_hashKey[16];

public:
	ROSCryptoState();

	inline const uint8_t* GetXorKey()
	{
		return m_xorKey;
	}

	inline const uint8_t* GetHashKey()
	{
		return m_hashKey;
	}
};

inline ROSCryptoState::ROSCryptoState()
{
	// initialize the key inputs
	uint8_t* platformStr = a_base64_decode(ROS_PLATFORM_KEY, strlen(ROS_PLATFORM_KEY));

	memcpy(m_rc4Key, &platformStr[1], sizeof(m_rc4Key));
	memcpy(m_xorKey, &platformStr[33], sizeof(m_xorKey));
	memcpy(m_hashKey, &platformStr[49], sizeof(m_hashKey));

	free(platformStr);

	// create the RC4 cipher and decode the keys
	m_rc4 = Botan::StreamCipher::create("RC4")->clone();

	// set the key
	m_rc4->set_key(m_rc4Key, sizeof(m_rc4Key));

	// decode the xor key
	m_rc4->cipher1(m_xorKey, sizeof(m_xorKey));

	// reset the key
	m_rc4->set_key(m_rc4Key, sizeof(m_rc4Key));

	// decode the hash key
	m_rc4->cipher1(m_hashKey, sizeof(m_hashKey));

	// and we're done
	delete m_rc4;
}


inline std::string GetROSVersionString()
{
	const char* baseString = "e=1,t=gta5,p=pcros,v=11";

	// create the XOR'd buffer
	std::vector<uint8_t> xorBuffer(strlen(baseString) + 4);

	// set the key for the XOR buffer
	*(uint32_t*)&xorBuffer[0] = 0xCDCDCDCD;

	for (int i = 4; i < xorBuffer.size(); i++)
	{
		xorBuffer[i] = baseString[i - 4] ^ 0xCD;
	}

	// base64 the string
	char* base64str = a_base64_encode(&xorBuffer[0], xorBuffer.size());

	// create a wide string version
	std::string str(base64str, 36);

	free(base64str);

	// return va() version of the base64 string
	return std::format("ros {}", str);
}

inline std::string EncryptROSData(const std::string& input, std::string sessionKey = "")
{
	// initialize state
	ROSCryptoState state;
	std::stringstream output;

	// decode session key, if needed
	bool hasSecurity = (!sessionKey.empty());

	uint8_t sessKey[16];

	if (hasSecurity)
	{
		auto keyData = Botan::base64_decode(sessionKey);
		memcpy(sessKey, keyData.data(), sizeof(sessKey));
	}

	// get a random RC4 key
	uint8_t rc4Key[16];

	Botan::AutoSeeded_RNG rng;
	rng.randomize(rc4Key, sizeof(rc4Key));

	// XOR the key with the global XOR key and write it to the output
	for (int i = 0; i < sizeof(rc4Key); i++)
	{
		char thisChar = rc4Key[i] ^ state.GetXorKey()[i];

		output << std::string(&thisChar, 1);

		if (hasSecurity)
		{
			rc4Key[i] ^= sessKey[i];
		}
	}

	// create a RC4 cipher for the data
	Botan::StreamCipher* rc4 = Botan::StreamCipher::create("RC4")->clone();
	rc4->set_key(rc4Key, sizeof(rc4Key));

	// encrypt the passed user data using the key
	std::vector<uint8_t> inData(input.size());
	memcpy(&inData[0], input.c_str(), inData.size());

	rc4->encipher(inData);

	// write the inData to the output stream
	output << std::string(reinterpret_cast<const char*>(&inData[0]), inData.size());

	// get a hash for the stream's content so far
	std::string tempContent = output.str();

	Botan::Buffered_Computation* sha1;

	if (!hasSecurity)
	{
		sha1 = Botan::HashFunction::create("SHA1")->clone();
	}
	else
	{
		auto hmac = Botan::MessageAuthenticationCode::create("HMAC(SHA1)")->clone();
		hmac->set_key(rc4Key, sizeof(rc4Key));

		sha1 = hmac;
	}

	sha1->update(reinterpret_cast<const uint8_t*>(tempContent.c_str()), tempContent.size());
	sha1->update(state.GetHashKey(), 16);

	auto hashData = sha1->final();

	// free the algorithms
	delete rc4;
	delete sha1;

	// and return the appended output
	return tempContent + std::string(reinterpret_cast<const char*>(&hashData[0]), hashData.size());
}

#pragma optimize("", off)
inline std::string DecryptROSData(const char* data, size_t size, const std::string& sessionKey = "")
{
	// initialize state
	ROSCryptoState state;

	// read the packet RC4 key from the packet
	uint8_t rc4Key[16];

	bool hasSecurity = (!sessionKey.empty());

	uint8_t sessKey[16];

	if (hasSecurity)
	{
		auto keyData = Botan::base64_decode(sessionKey);
		memcpy(sessKey, keyData.data(), sizeof(sessKey));
	}

	for (int i = 0; i < sizeof(rc4Key); i++)
	{
		rc4Key[i] = data[i] ^ state.GetXorKey()[i];

		if (hasSecurity)
		{
			rc4Key[i] ^= sessKey[i];
		}
	}

	// initialize RC4 with the packet key
	Botan::StreamCipher* rc4 = Botan::StreamCipher::create("RC4")->clone();
	rc4->set_key(rc4Key, sizeof(rc4Key));

	// read the block size from the data
	uint8_t blockSizeData[4];
	uint8_t blockSizeDataLE[4];
	rc4->cipher(reinterpret_cast<const uint8_t*>(&data[16]), blockSizeData, 4);

	// swap endianness
	blockSizeDataLE[3] = blockSizeData[0];
	blockSizeDataLE[2] = blockSizeData[1];
	blockSizeDataLE[1] = blockSizeData[2];
	blockSizeDataLE[0] = blockSizeData[3];

	uint32_t blockSize = (*(uint32_t*)&blockSizeDataLE) + 20;

	// create a buffer for the block
	std::vector<uint8_t> blockData(blockSize);

	// a result stringstream as well
	std::stringstream result;

	// loop through packet blocks
	size_t start = 20;

	while (start < size)
	{
		// calculate the end of this block
		int end = min(size, start + blockSize);

		// remove the size of the SHA1 hash from the end
		end -= 20;

		int thisLen = end - start;

		if (thisLen < 0)
		{
			return std::string{
				data, size
			};
		}

		// decrypt the block
		rc4->cipher(reinterpret_cast<const uint8_t*>(&data[start]), &blockData[0], thisLen);

		// TODO: compare the resulting hash

		// append to the result buffer
		result << std::string(reinterpret_cast<const char*>(&blockData[0]), thisLen);

		// increment the counter
		start += blockSize;
	}

	delete rc4;

	return result.str();
}
#pragma optimize("", on)

inline string url_encode(const string& value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
	{
		std::string::value_type c = (*i);
		if (((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-' || c == '_' || c == '.' || c == '~') && c != '+')
		{
			escaped << c;
		}
		else if (c == ' ')
		{
			escaped << '+';
		}
		else
		{
			escaped << '%' << std::setw(2) << ((int)(uint8_t)c) << std::setw(0);
		}
	}

	return string(escaped.str().c_str());
}


inline std::string BuildPostString(const std::map<std::string, std::string>& fields)
{
	std::stringstream retval;

	for (auto const& [key, val] : fields)
	{
		retval << key << "=" << url_encode(val) << "&";
	}

	string str = string(retval.str().c_str());
	return str.substr(0, str.length() - 1);
}

template<typename TAlloc>
inline auto HeadersHmac(const std::vector<uint8_t, TAlloc>& challenge, const char* method, const std::string& path, const std::string& sessionKey, const std::string& sessionTicket)
{
	auto hmac = std::unique_ptr<Botan::MessageAuthenticationCode>(Botan::MessageAuthenticationCode::create("HMAC(SHA1)")->clone());

	ROSCryptoState cryptoState;

	// set the key
	uint8_t hmacKey[16];

	// xor the RC4 key with the platform key (and optionally the session key)
	auto rc4Xor = a_base64_decode(sessionKey.c_str(), sizeof(sessionKey));

	for (int i = 0; i < sizeof(hmacKey); i++)
	{
		hmacKey[i] = rc4Xor[i] ^ cryptoState.GetXorKey()[i];
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
	hmac->update(cryptoState.GetHashKey(), 16);

	// set the request header
	auto hmacValue = hmac->final();

	return hmacValue;
}

inline string urlDecode(string SRC) {
	string ret;
	char ch;
	int i, ii;
	for (i = 0; i < SRC.length(); i++) {
		if (SRC[i] == '%') {
			sscanf_s(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
		else {
			ret += SRC[i];
		}
	}
	return (ret);
}
