#pragma once
#include <string>
#include "../imgui/imgui.h"

namespace config
{
	void loadConfig();
	void saveConfig();
}

namespace config
{
	inline bool saveQueue = true;
	inline bool saveArgs = true;
	inline bool saveDir = true;
	inline char configName[128] = "Enter config here";
	inline std::string appdata;
	inline std::string configDir;
}