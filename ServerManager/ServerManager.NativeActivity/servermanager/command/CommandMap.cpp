#include "CommandMap.h"
#include "../Server.h"
#include "defaults/GameModeCommand.h"
#include "defaults/WhitelistCommand.h"
#include "defaults/HelpCommand.h"
#include "defaults/KickCommand.h"
#include "defaults/BanCommand.h"
#include "defaults/BanIpCommand.h"
#include "defaults/BanListCommand.h"
#include "defaults/PardonCommand.h"
#include "defaults/PardonIpCommand.h"
#include "defaults/OpCommand.h"
#include "defaults/DeopCommand.h"
#include "defaults/TellCommand.h"
#include "defaults/ListCommand.h"
#include "defaults/TimeCommand.h"
#include "defaults/GiveCommand.h"
#include "defaults/TeleportCommand.h"
#include "defaults/MeCommand.h"
#include "defaults/KillCommand.h"
#include "../util/SMUtil.h"

CommandMap::CommandMap()
{
	setDefaultCommands();
}

CommandMap::~CommandMap()
{
	for (int i = 0; i < commands.size(); ++i)
		delete commands[i];

	knownCommands.clear();
	commands.clear();
}

void CommandMap::setDefaultCommands()
{
	registerCommand("servermanager", new HelpCommand);
	registerCommand("servermanager", new GameModeCommand);
	registerCommand("servermanager", new WhitelistCommand);
	registerCommand("servermanager", new KickCommand);
	registerCommand("servermanager", new BanCommand);
	registerCommand("servermanager", new BanIpCommand);
	registerCommand("servermanager", new BanListCommand);
	registerCommand("servermanager", new PardonCommand);
	registerCommand("servermanager", new PardonIpCommand);
	registerCommand("servermanager", new OpCommand);
	registerCommand("servermanager", new DeopCommand);
	registerCommand("servermanager", new TellCommand);
	registerCommand("servermanager", new ListCommand);
	registerCommand("servermanager", new TimeCommand);
	registerCommand("servermanager", new GiveCommand);
	registerCommand("servermanager", new TeleportCommand);
	registerCommand("servermanager", new MeCommand);
	registerCommand("servermanager", new KillCommand);
}

void CommandMap::setFallbackCommands()
{

}

void CommandMap::registerAll(const std::string &fallbackPrefix, std::vector<Command *> commands)
{
	if (!commands.empty())
	{
		for (Command *c : commands)
			registerCommand(fallbackPrefix, c);
	}
}

bool CommandMap::registerCommand(const std::string &fallbackPrefix, Command *command)
{
	return registerCommand(command->getName(), fallbackPrefix, command);
}

bool CommandMap::registerCommand(const std::string &label, const std::string &fallbackPrefix, Command *command)
{
	std::string newLabel = SMUtil::toLower(SMUtil::trim(label));
	std::string newFallbackPrefix = SMUtil::toLower(SMUtil::trim(fallbackPrefix));

	bool registered = registerCommand(newLabel, command, false, newFallbackPrefix);

	std::vector<std::string> aliases = command->getAliases();
	for (auto it = aliases.begin(); it != aliases.end(); ++it)
	{
		if (!registerCommand(*it, command, true, newFallbackPrefix))
			it = aliases.erase(it);
	}

	if (!registered)
		command->setLabel(newFallbackPrefix + ":" + newLabel);

	command->registerCommand(this);

	return registered;
}

bool CommandMap::registerCommand(const std::string &label, Command *command, bool isAlias, const std::string &fallbackPrefix)
{
	knownCommands[fallbackPrefix + ":" + label] = command;
	if ((command->isVanillaCommand() || isAlias) && knownCommands.find(label) != knownCommands.end())
		return false;

	if (knownCommands.find(label) != knownCommands.end() && !command->getLabel().compare(label))
		return false;

	if (!isAlias)
	{
		commands.push_back(command);
		command->setLabel(label);
	}
	knownCommands[label] = command;

	return true;
}

bool CommandMap::dispatch(SMPlayer *sender, const std::string &cmdLine)
{
	std::vector<std::string> args = SMUtil::split(cmdLine, ' ');
	if (args.empty())
		return false;

	std::string sentCommandLabel = SMUtil::toLower(args.front());
	Command *target = getCommand(sentCommandLabel);

	if (!target)
		return false;

	args.erase(args.begin());
	target->execute(sender, sentCommandLabel, args);

	return true;
}

void CommandMap::clearCommands()
{
	for (auto &it : knownCommands)
		it.second->unregister(this);

	for (Command *cmd : commands)
		delete cmd;

	knownCommands.clear();
	commands.clear();
	setDefaultCommands();
}

Command *CommandMap::getCommand(const std::string &name)
{
	std::string lname = SMUtil::toLower(name);
	if (knownCommands.find(lname) != knownCommands.end())
		return knownCommands[lname];

	return NULL;
}

const std::map<std::string, Command *> &CommandMap::getCommands() const
{
	return knownCommands;
}
