#include "message.h"


using namespace rapidjson;
using namespace std;

string Message::json() const
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();

	writer.String("body");
    writer.String(m_msg);

	writer.EndObject();
	return string(s.GetString());
}