#include <map>

#include "PluginCommandJsonParser.h"
#include "PluginCommand.h"
#include "../plugin/Plugin.h"
#include "../plugin/PluginDescriptionFile.h"

std::vector<Command *> PluginCommandJsonParser::parse(Plugin *plugin)
{
	std::vector<Command *> pluginCmds;

	std::map<std::string, std::map<std::string, PluginDescriptionFile::CommandDescValue>> cmdMap = plugin->getDescription()->getCommands();
	for (auto &it : cmdMap)
	{
		if (it.first.find(':') != std::string::npos)
			continue;

		Command *newCmd = new PluginCommand(it.first, plugin);
		if (it.second.find("description") != it.second.end())
		{
			PluginDescriptionFile::CommandDescValue description = it.second["description"];
			newCmd->setDescription(description.strValue);
		}
		
		if (it.second.find("usage") != it.second.end())
		{
			PluginDescriptionFile::CommandDescValue usage = it.second["usage"];
			newCmd->setUsage(usage.strValue);
		}
		
		if (it.second.find("usage") != it.second.end())
		{
			PluginDescriptionFile::CommandDescValue aliases = it.second["aliases"];
			std::vector<std::string> aliasList;
			if (aliases.isArray)
			{
				for (std::string alias : aliases.arrayValue)
				{
					if (alias.find(':') != std::string::npos)
						continue;

					aliasList.push_back(alias);
				}
			}
			else
			{
				if (aliases.strValue.find(':') == std::string::npos)
					aliasList.push_back(aliases.strValue);
			}
			newCmd->setAliases(aliasList);
		}

		if (it.second.find("permission") != it.second.end())
		{
			PluginDescriptionFile::CommandDescValue permission = it.second["permission"];
			newCmd->setPermission(permission.strValue);
		}

		if (it.second.find("permission-message") != it.second.end())
		{
			PluginDescriptionFile::CommandDescValue permissionMessage = it.second["permission-message"];
			newCmd->setPermissionMessage(permissionMessage.strValue);
		}
		pluginCmds.push_back(newCmd);
	}
	return pluginCmds;
}