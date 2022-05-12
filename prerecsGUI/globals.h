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
	std::string convdir;
	std::string cnsl;
	int startOption;
	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_MultipleSelection);
	ImGui::FileBrowser folderDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_MultipleSelection);
	std::vector<std::string> locations = {};
	char fname[128];
	ImFont* arialBig;
	ImFont* arial;
	ImFont* bigIcon;
};

inline c_globals globals;