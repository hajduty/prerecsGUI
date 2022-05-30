#include "config.h"
#include "miniconf.h"
#include <cstdio>
#include <string>
#include <direct.h>
#include <filesystem>
#include "../globals.h"
#include "../imgui/imgui.h"

void config::saveConfig() {

	miniconf::Config conf;

	config::configDir = config::appdata + "\\configs\\";

	std::string configPath = config::configDir + config::configName + ".json";

	if (config::configName[0] == '\0')
		return;

	if (!(std::filesystem::exists(config::configDir)))
		_mkdir(config::configDir.c_str());

	conf.description("prerecsGUI config");

	conf.option("saveArgs").defaultValue(true).required(true).description("");
	conf.option("args").defaultValue("").required(false).description("");

	conf.option("saveQueue").defaultValue(true).required(true).description("");
	conf.option("queue").defaultValue("").required(false).description("");

	conf.option("saveDir").defaultValue(true).required(true).description("");
	conf.option("dir").defaultValue("").required(false).description("");

	if (conf.parse(__argc, __argv)) 
	{
		conf["saveArgs"] = config::saveArgs;
		conf["saveQueue"] = config::saveQueue;
		conf["saveDir"] = config::saveDir;		

		if (config::saveQueue)
		{
			std::string locationString;
		
			std::string loc;
		
			for (int i = 0; i < globals.locations.size(); i++)
			{
				if (!(globals.locationsDisplay.at(i).find("Finished") != std::string::npos))
					loc += globals.locations.at(i) + ";";
			}
			conf["queue"] = loc;
		}

		if (config::saveDir)
		{
			conf["dir"] = globals.dir;
		}

		if (config::saveArgs)
		{
			conf["args"] = globals.args;
		}
		std::string n = config::configName;
		std::string nam = n + ".json";

		conf.serialize(configPath, miniconf::Config::ExportFormat::JSON);
		
		conf.log();
	}
}

void config::loadConfig()
{
	miniconf::Config conf;

	config::configDir = config::appdata + "\\configs\\";


	std::string configPath = config::configDir + config::configName;

	if (!(configPath.find(".json") != std::string::npos) | !std::filesystem::exists(configPath))
		return;

	conf.config(configPath);
	
	if (conf["saveArgs"].getBoolean() && config::saveArgs) 
	{
		globals.args = conf["args"].getString();
	}

	if (conf["saveDir"].getBoolean() && config::saveDir) 
	{
		globals.dir = conf["dir"].getString();
		strcpy(globals.fname, globals.dir.c_str());
	}

	if (conf["saveQueue"].getBoolean() && config::saveQueue)
	{
		std::string str = conf["queue"].getString();
		char* point;
		point = strtok((char*)str.c_str(), ";");

		while (point != NULL)
		{
			std::cout << point;
			std::string r = point;
			std::string rr = r.substr(r.find_last_of("/\\") + 1);
			
			globals.locations.push_back(point);
			globals.locationsDisplay.push_back(rr);

			point = strtok(NULL, ";");
		}
	}
}