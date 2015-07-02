#include "message.h"


#define RAPIDJSON_HAS_STDSTRING 1
using namespace rapidjson;
using namespace std;

string Message::json() const
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();

	writer.EndObject();
	return string(s.GetString());
}