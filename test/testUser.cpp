#include "../src/user.h"
#include <cassert>
#include <iostream>

using namespace std;
void testUserJson()
{
	User user1("saye");
	auto jsonUser1 = user1.json();
	assert(jsonUser1 == "{\"name\":\"saye\"}");
	std::cout << jsonUser1 << std::endl;
}

int main()
{
	testUserJson();
	return 0;
}