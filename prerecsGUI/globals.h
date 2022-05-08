#pragma once
#include <unordered_map>
#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"

class c_globals {
public:
	bool active = true;
	bool stopEncode = false;
	bool start = true;

	std::string startBtn = "START";
	std::string appdata;
	std::string dir;
	std::string file[99];
	std::string codec = "xvid";
	int tab = 2;
	std::string convdir;
	std::string cnsl;

	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_MultipleSelection);
	std::vector<std::string> locations = {};
	char fname[128];
	ImFont* arialBig;
};

inline c_globals globals;