#include "self_implement_functions.h"

#include <sstream>
#include <string>

std::string IntToStr(int digit, int showdists)
{
	std::string str;
	std::stringstream ss;
	ss << digit;
	ss >> str;
	if (str.size() < showdists)
	{
		for (int i = 0; i < showdists - str.size(); ++i)
		{
			str = "0" + str;
		}
	}
	return str;
}