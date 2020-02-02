// autobot.cpp : Defines the entry point for the application.
//

#include "autobot.h"
#include <map>

std::pair<int, int> divide(int x, int y) {
	return std::make_pair(x / y, y / x);
}

int main()
{
	std::cout << "test" << std::endl;
	return 0;
}
