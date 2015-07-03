#ifndef UVCHATROOM_MESSAGE_HEADER
#define UVCHATROOM_MESSAGE_HEADER

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>


class Message
{
private:
    std::string m_msg;
public:
	typedef enum {
		UserJoin = 1,
		TextMessage,
		UserLeave,
	} MessageType;
public:
    Message(std::string msg) :
            m_msg(msg)
    {

    }
	std::string json() const;
};

#endif