#ifndef UVCHATROOM_MESSAGE_HEADER
#define UVCHATROOM_MESSAGE_HEADER

#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


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
	
};

#endif