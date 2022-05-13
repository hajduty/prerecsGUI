#pragma once
#include <unordered_map>
#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"

class c_globals {
public:
	bool active = true;
	bool stopEncode = false;
	bool start = true;
	int startOption = 3;
	char fname[128];

	std::vector<std::string> locations = {};
	std::string locationsDisplay[99];
	std::string startBtn = "START";
	std::string codec = "xvid";
	std::string appdata;
	std::string dir;
	std::string convdir;
	std::string cnsl;

	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_MultipleSelection);
	ImGui::FileBrowser folderDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_MultipleSelection);

	ImFont* arialBig;
	ImFont* arial;
	ImFont* bigIcon;
};

inline c_globals globals;