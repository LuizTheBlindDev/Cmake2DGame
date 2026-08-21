#pragma once
#include <string>

std::string GetResourceDir()
{
#ifdef NDEBUG 
	return "assets/";
#else
	return "C:/Users/luizg/source/games C++/LightYears/LightYearsGame/assets/";
#endif
}
