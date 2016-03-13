#include "TimeCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../level/SMLevel.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"
#include "minecraftpe/world/level/Level.h"

TimeCommand::TimeCommand()
	: VanillaCommand("time")
{
	description = "Changes the time on each world";
	usageMessage = "/time <set|add> <value> OR /time <start|stop|query>";
	setPermission("servermanager.command.time.add;servermanager.command.time.set;servermanager.command.time.query;servermanager.command.time.start;servermanager.command.time.stop");
}

bool TimeCommand::execute(CommandSender *sender, std::string &commandLabel, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if ((int)args.size() < 1)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	if (!args[0].compare("start"))
	{
		if (badPerm(sender, "start")) return true;
		ServerManager::getLevel()->getHandle()->setDayCycleActive(true);
		Command::broadcastCommandMessage(sender, "Restarted the time");
	}
	else if (!args[0].compare("stop"))
	{
		if (badPerm(sender, "stop")) return true;
		ServerManager::getLevel()->getHandle()->setDayCycleActive(false);
		Command::broadcastCommandMessage(sender, "Stopped the time");
	}
	else if (!args[0].compare("query"))
	{
		if (badPerm(sender, "query")) return true;
		sender->sendTranslation("commands.time.query", { SMUtil::toString(ServerManager::getLevel()->getHandle()->getTime()) });
	}
	else if (!args[0].compare("set"))
	{
		if (badPerm(sender, "set")) return true;
		if ((int)args.size() < 2)
		{
			sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
			return false;
		}

		int value;

		if (!args[1].compare("day"))
			value = 0;
		else if (!args[1].compare("night"))
			value = 14000;
		else
			value = getInteger(sender, args[1], 0);

		ServerManager::getLevel()->getHandle()->setTime(value);
		Command::broadcastCommandTranslation(sender, "commands.time.set", { SMUtil::toString(value) });
	}
	else if (!args[0].compare("add"))
	{
		if (badPerm(sender, "add")) return true;
		if ((int)args.size() < 2)
		{
			sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
			return false;
		}

		int value = getInteger(sender, args[1], 0);

		ServerManager::getLevel()->getHandle()->setTime(ServerManager::getLevel()->getHandle()->getTime() + value);
		Command::broadcastCommandTranslation(sender, "commands.time.added", { SMUtil::toString(value) });
	}
	else
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });

	return true;
}

bool TimeCommand::badPerm(CommandSender *sender, const std::string &perm)
{
	if (!sender->hasPermission("servermanager.command.time." + perm))
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.permission", {});
		return true;
	}
	return false;
}