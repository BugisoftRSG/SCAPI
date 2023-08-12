#include "ros_crypt.hpp"
#include <sstream>
#include <iomanip>
#include <format>
#include <algorithm>
#include <iostream>

ROSCryptoState::ROSCryptoState(const string& platformKey)
{
	Botan::secure_vector<uint8_t> platformStr = Botan::base64_decode(platformKey);

	memcpy(m_rc4Key, &platformStr[1], sizeof(m_rc4Key));
	memcpy(m_xorKey, &platformStr[33], sizeof(m_xorKey));
	memcpy(m_hashKey, &platformStr[49], sizeof(m_hashKey));

	m_rc4 = Botan::StreamCipher::create("RC4")->clone();
	m_rc4->set_key(m_rc4Key, sizeof(m_rc4Key));
	m_rc4->cipher1(m_xorKey, sizeof(m_xorKey));
	m_rc4->set_key(m_rc4Key, sizeof(m_rc4Key));
	m_rc4->cipher1(m_hashKey, sizeof(m_hashKey));

	delete m_rc4;
}

ROSCryptoState::~ROSCryptoState()
{
    delete m_rc4;
}

const uint8_t* ROSCryptoState::GetXorKey() const
{
    return m_xorKey;
}

const uint8_t* ROSCryptoState::GetHashKey() const
{
    return m_hashKey;
}

string ROSCrypt::DecryptROSData(const char* data, size_t size, const string& sessionKey)
{
	auto cryptoState = m_cryptoState.get();

	uint8_t rc4Key[16];

	bool hasSecurity = (!sessionKey.empty());

	uint8_t sessKey[16];

	if (hasSecurity)
	{
		auto keyData = Botan::base64_decode(sessionKey.data());
		memcpy(sessKey, keyData.data(), sizeof(sessKey));
	}

	for (int i = 0; i < sizeof(rc4Key); i++)
	{
		rc4Key[i] = data[i] ^ cryptoState->GetXorKey()[i];

		if (hasSecurity)
		{
			rc4Key[i] ^= sessKey[i];
		}
	}

	Botan::StreamCipher* rc4 = Botan::StreamCipher::create("RC4")->clone();
	rc4->set_key(rc4Key, sizeof(rc4Key));

	uint8_t blockSizeData[4];
	uint8_t blockSizeDataLE[4];
	rc4->cipher(reinterpret_cast<const uint8_t*>(&data[16]), blockSizeData, 4);

	blockSizeDataLE[3] = blockSizeData[0];
	blockSizeDataLE[2] = blockSizeData[1];
	blockSizeDataLE[1] = blockSizeData[2];
	blockSizeDataLE[0] = blockSizeData[3];

	uint32_t blockSize = (*(uint32_t*)&blockSizeDataLE) + 20;

	vector<uint8_t> blockData(blockSize);

	stringstream result;

	size_t start = 20;

	while (start < size)
	{
		int end = min(size, start + blockSize);

		end -= 20;

		int thisLen = end - start;

		if (thisLen < 0)
		{
			return string{data, size};
		}

		rc4->cipher(reinterpret_cast<const uint8_t*>(&data[start]), &blockData[0], thisLen);

		result << string(reinterpret_cast<const char*>(&blockData[0]), thisLen);

		start += blockSize;
	}

	delete rc4;

	return result.str();
}

string ROSCrypt::EncryptROSData(const string& input, const string& sessionKey)
{
	auto state = m_cryptoState.get();
	stringstream output;

	bool hasSecurity = (!sessionKey.empty());

	uint8_t sessKey[16];

	if (hasSecurity)
	{
		auto keyData = Botan::base64_decode(sessionKey);
		memcpy(sessKey, keyData.data(), sizeof(sessKey));
	}

	uint8_t rc4Key[16];

	Botan::AutoSeeded_RNG rng;
	rng.randomize(rc4Key, sizeof(rc4Key));

	for (int i = 0; i < sizeof(rc4Key); i++)
	{
		char thisChar = rc4Key[i] ^ state->GetXorKey()[i];

		output << string(&thisChar, 1);

		if (hasSecurity)
		{
			rc4Key[i] ^= sessKey[i];
		}
	}

	Botan::StreamCipher* rc4 = Botan::StreamCipher::create("RC4")->clone();
	rc4->set_key(rc4Key, sizeof(rc4Key));

	vector<uint8_t> inData(input.size());
	memcpy(&inData[0], input.c_str(), inData.size());

	rc4->encipher(inData);

	output << string(reinterpret_cast<const char*>(&inData[0]), inData.size());

	string tempContent = output.str();

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
	sha1->update(state->GetHashKey(), 16);

	auto hashData = sha1->final();

	delete rc4;
	delete sha1;

	return tempContent + string(reinterpret_cast<const char*>(&hashData[0]), hashData.size());
}

string ROSCrypt::url_encode(const string& value)
{
	ostringstream escaped;
	escaped.fill('0');
	escaped << hex;

	for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
	{
		string::value_type c = (*i);
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
			escaped << '%' << setw(2) << ((int)(uint8_t)c) << setw(0);
		}
	}

	return string(escaped.str().c_str());
}

string ROSCrypt::ex_url_encode(const string& value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : value) {
		if (static_cast<unsigned char>(c) <= 127) {
			// Encode non-alphanumeric characters except '-' and '_'
			if (std::isalnum(c) || c == '-' || c == '_') {
				escaped << c;
			}
			else {
				escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
			}
		}
		else {
			// Handle non-ASCII characters properly by encoding as UTF-8
			std::stringstream utf8Encoded;
			utf8Encoded << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
			for (size_t i = 0; i < utf8Encoded.str().size(); ++i) {
				escaped << '%' << utf8Encoded.str()[i];
			}
		}
	}

	return escaped.str();
}

string ROSCrypt::BuildPostString(const map<string, string>& fields)
{
	stringstream retval;

	for (auto& field : fields)
	{
		retval << field.first << "=" << url_encode(field.second) << "&";
	}

	string str = string(retval.str().c_str());
	return str.substr(0, str.length() - 1);
}

string ROSCrypt::GetROSVersionString()
{
	string baseString;
	if (m_launcher)
		baseString = "e=1,t=launcher,p=pcros,v=11";
	else
		baseString = "e=1,t=gta5,p=pcros,v=11";

	vector<uint8_t> xorBuffer(baseString.length() + 4);

	if (m_launcher)
	{
		*(uint32_t*)&xorBuffer[0] = 0xC5C5C5C5;

		for (int i = 4; i < xorBuffer.size(); i++)
		{
			xorBuffer[i] = baseString[i - 4] ^ 0xC5;
		}
	}
	else
	{
		*(uint32_t*)&xorBuffer[0] = 0xCDCDCDCD;

		for (int i = 4; i < xorBuffer.size(); i++)
		{
			xorBuffer[i] = baseString[i - 4] ^ 0xCD;
		}
	}

	string base64str = Botan::base64_encode(reinterpret_cast<const Botan::byte*>(&xorBuffer[0]), xorBuffer.size());

	return format("ros {}", base64str);
}
