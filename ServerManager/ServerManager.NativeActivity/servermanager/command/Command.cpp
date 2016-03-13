#include "Command.h"
#include "../ServerManager.h"
#include "../entity/player/SMPlayer.h"
#include "../level/SMLevel.h"
#include "../command/CommandMap.h"
#include "../plugin/PluginManager.h"
#include "../util/SMUtil.h"
#include "../ChatColor.h"
#include "minecraftpe/locale/I18n.h"

Command::Command(const std::string &name, const std::string &description, const std::string &usageMessage, const std::vector<std::string> &aliases)
{
	commandMap = NULL;
	this->name = name;
	nextLabel = name;
	this->label = name;
	this->description = description;
	this->usageMessage = usageMessage.empty() ? "#" + name : usageMessage;
	this->aliases = aliases;
	activeAliases = aliases;
}

Command::~Command()
{
}

bool Command::isVanillaCommand() const
{
	return false;
}

bool Command::isPluginCommand() const
{
	return false;
}

const std::string &Command::getName() const
{
	return name;
}

const std::string &Command::getDescription() const
{
	return description;
}

void Command::setDescription(const std::string &description)
{
	this->description = description;
}

const std::string &Command::getPermission() const
{
	return permission;
}

void Command::setPermission(const std::string &permission)
{
	this->permission = permission;
}

bool Command::testPermission(CommandSender *target)
{
	if (testPermissionSilent(target))
		return true;

	if (permissionMessage.empty())
		target->sendTranslation(ChatColor::RED + "%commands.generic.permission", {});
	else
	{
		std::string message = permissionMessage;
		if (message.find("<permission>") != std::string::npos)
		{
			std::string findString = "<permission>";
			message.replace(message.find(findString), findString.length(), permission);
		}

		for (std::string line : SMUtil::split(message, '\n'))
			target->sendMessage(line);
	}
	return false;
}

bool Command::testPermissionSilent(CommandSender *target)
{
	if (permission.empty())
		return true;

	for (std::string p : SMUtil::split(permission, ';'))
		if (target->hasPermission(p))
			return true;

	return false;
}

const std::string &Command::getPermissionMessage() const
{
	return permissionMessage;
}

void Command::setPermissionMessage(const std::string &permissionMessage)
{
	this->permissionMessage = permissionMessage;
}

std::vector<std::string> Command::getAliases() const
{
	return activeAliases;
}

bool Command::setAliases(const std::vector<std::string> &aliases)
{
	this->aliases = aliases;
	if (!isRegistered())
		activeAliases = aliases;

	return true;
}

bool Command::registerCommand(CommandMap *commandMap)
{
	if (allowChangesFrom(commandMap))
	{
		this->commandMap = commandMap;
		return true;
	}
	return false;
}

bool Command::unregister(CommandMap *commandMap)
{
	if (allowChangesFrom(commandMap))
	{
		delete commandMap;
		commandMap = NULL;
		activeAliases = aliases;
		label = nextLabel;
		return true;
	}
	return false;
}

bool Command::isRegistered() const
{
	return commandMap != NULL;
}

bool Command::allowChangesFrom(CommandMap *commandMap)
{
	return !this->commandMap || this->commandMap == commandMap;
}

bool Command::setLabel(const std::string &name)
{
	nextLabel = label;
	if (!isRegistered())
	{
		label = name;
		return true;
	}
	return false;
}

std::string Command::getLabel() const
{
	return label;
}

void Command::setUsage(const std::string &usage)
{
	usageMessage = usage;
}

std::string Command::getUsage() const
{
	return usageMessage;
}

void Command::broadcastCommandMessage(CommandSender *source, const std::string &message, bool sendToSource)
{
	std::vector<Permissible *> users = ServerManager::getPluginManager()->getPermissionSubscriptions(Server::BROADCAST_CHANNEL_ADMINISTRATIVE);
	std::string colored = ChatColor::GRAY + ChatColor::ITALIC + "[" + source->getName() + ": " + message + "]";

	for (size_t i = 0; i < users.size(); ++i)
	{
		Permissible *user = users[i];
		if (user->isCommandSender())
		{
			CommandSender *target = (CommandSender *)user;
			if (target == source && !sendToSource)
				continue;

			target->sendMessage(colored);
		}
	}
}

void Command::broadcastCommandTranslation(CommandSender *source, const std::string &message, const std::vector<std::string> &params, bool sendToSource)
{
	std::vector<Permissible *> users = ServerManager::getPluginManager()->getPermissionSubscriptions(Server::BROADCAST_CHANNEL_ADMINISTRATIVE);
	std::string colored = ChatColor::GRAY + ChatColor::ITALIC + "[" + source->getName() + ": " + (I18n::get(message).compare(message) ? "%" : "") + message + "]";
	
	for (size_t i = 0; i < users.size(); ++i)
	{
		Permissible *user = users[i];
		if (user->isCommandSender())
		{
			CommandSender *target = (CommandSender *)user;
			if (target == source && !sendToSource)
				continue;

			target->sendTranslation(colored, params);
		}
	}
}
