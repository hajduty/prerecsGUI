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
	char argsDisplay[128];

	std::vector<std::string> locations = {};
	std::vector<std::string> locationsDisplay = {};
	std::vector<std::string> cnsl = {};

	std::string startBtn = "START";
	std::string codec = "xvid";
	std::string appdata;
	std::string dir;
	std::string convdir;
	std::string args = "\" -c:v mpeg4 -vtag xvid -qscale:v 1 -qscale:a 1 -g 32 -vsync 1 -y \"";
	std::string filetype = ".avi";

	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_MultipleSelection);
	ImGui::FileBrowser folderDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_MultipleSelection);

	ImFont* arialBig;
	ImFont* arialMedium;
	ImFont* arialSmall;
	ImFont* arial;
	ImFont* bigIcon;
};

inline c_globals globals;