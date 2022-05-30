#pragma once
#include <iostream>

namespace prerecs
{
	void startEncode();
}

namespace prerecs
{
	inline std::string exec(const char* cmd);
	inline std::string subFolderPath;
	inline bool success;
}