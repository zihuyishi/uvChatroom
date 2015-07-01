#ifndef UVCHATROOM_MESSAGE_HEADER
#define UVCHATROOM_MESSAGE_HEADER

#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


class Message
{
public:
	typedef enum {
		UserJoin,
		TextMessage,
		UserLeave,
	} MessageType;
	
};

#endif