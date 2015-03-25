#ifndef UVCHATROOM_USER_HEADER
#define UVCHATROOM_USER_HEADER
#include <string>

#define RAPIDJSON_HAS_STDSTRING
#include "../include/rapidjson/rapidjson.h"
class User
{
	std::string m_name;
public:
	User(const char* name) {
		m_name = std::string(name);
	}
	User(const User &other) {
		m_name = other.m_name;
	}
	User& operator=(const User &other) {
		m_name = other.m_name;
	}
public:
	std::string json() {
		StringBuffer s;
		Writer<StringBuffer> writer(s);

		writer.StartObject();
		writer.String("name");
		writer.String(m_name);
		return std::string(writer.GetString());
	}
};

#endif