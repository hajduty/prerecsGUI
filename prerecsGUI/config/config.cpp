#include "config.h"
#include "miniconf.h"
#include <cstdio>
#include <string>
#include <direct.h>
#include <filesystem>

#include <filesystem>
#include <utility>
#include <direct.h>

#include "../globals.h"
#include "../imgui/imgui.h"

void config::saveConfig() 
{
	miniconf::Config conf;
	config::configDir = config::appdata + "\\configs\\";
	std::string c = config::configName;
	std::string configPath;

	if (c.find(".json") != std::string::npos) 
	{
		configPath = config::configDir + config::configName;
	}
	else
	{
		configPath = config::configDir + config::configName + ".json";
	}

	if (config::configName[0] == '\0')
		return;

	if (!(std::filesystem::exists(config::configDir)))
		_mkdir(config::configDir.c_str());

	conf.description("prerecsGUI config");

	conf.option("ffmpeg.saveArgs").shortflag("x").defaultValue(true).required(true).description("");
	conf.option("ffmpeg.args").shortflag("a").defaultValue("").required(false).description("");

	conf.option("queue.saveQueue").shortflag("y").defaultValue(true).required(true).description("");
	conf.option("queue.queueList").shortflag("b").defaultValue("").required(false).description("");

	conf.option("outputDir.saveDir").shortflag("z").defaultValue(true).required(true).description("");
	conf.option("outputDir.dir").shortflag("c").defaultValue("").required(false).description("");

	if (conf.parse(__argc, __argv)) 
	{
		conf["ffmpeg.saveArgs"] = config::saveArgs;
		conf["queue.saveQueue"] = config::saveQueue;
		conf["outputDir.saveDir"] = config::saveDir;		

		if (config::saveQueue)
		{
			std::string locationString;
		
			std::string loc;
		
			for (int i = 0; i < globals.locations.size(); i++)
			{
				if (!(globals.locationsDisplay.at(i).find("Finished") != std::string::npos))
					loc += globals.locations.at(i) + ";";
			}
			conf["queue.queueList"] = loc;
		}

		if (config::saveDir)
		{
			conf["outputDir.dir"] = globals.dir;
		}

		if (config::saveArgs)
		{
			conf["ffmpeg.args"] = globals.args;
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
	
	if (conf["ffmpeg.saveArgs"].getBoolean() && config::saveArgs) 
	{
		globals.args = conf["ffmpeg.args"].getString();
	}

	if (conf["outputDir.saveDir"].getBoolean() && config::saveDir) 
	{
		globals.dir = conf["outputDir.dir"].getString();
		strcpy(globals.fname, globals.dir.c_str());
	}

	if (conf["queue.saveQueue"].getBoolean() && config::saveQueue)
	{
		std::string str = conf["queue.queueList"].getString();
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