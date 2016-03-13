#include "WhitelistCommand.h"
#include "../../ServerManager.h"
#include "../../SMList.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

WhitelistCommand::WhitelistCommand()
	: VanillaCommand("whitelist")
{
	description = "Manages the list of players allowed to use this server";
	usageMessage = "%commands.whitelist.usage";
	setPermission("servermanager.command.whitelist.reload;servermanager.command.whitelist.enable;servermanager.command.whitelist.disable;servermanager.command.whitelist.list;servermanager.command.whitelist.add;servermanager.command.whitelist.remove");
}

bool WhitelistCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	if (args.empty() || (int)args.size() > 2)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return true;
	}

	if ((int)args.size() == 1)
	{
		if (!args[0].compare("reload"))
		{
			if (badPerm(sender, "reload")) return true;

			ServerManager::reloadWhitelist();
			Command::broadcastCommandTranslation(sender, "commands.whitelist.reloaded", {});
			return true;
		}
		else if (!args[0].compare("on"))
		{
			if (badPerm(sender, "enable")) return true;

			ServerManager::setWhitelist(true);
			Command::broadcastCommandTranslation(sender, "commands.whitelist.enabled", {});
			return true;
		}
		else if (!args[0].compare("off"))
		{
			if (badPerm(sender, "disable")) return true;

			ServerManager::setWhitelist(false);
			Command::broadcastCommandTranslation(sender, "commands.whitelist.disabled", {});
			return true;
		}
		else if (!args[0].compare("list"))
		{
			if (badPerm(sender, "list")) return true;

			std::string result;

			std::vector<std::string> players = ServerManager::getWhitelist()->getAll();
			for (size_t i = 0; i < players.size(); i++)
			{
				if (i > 0)
					result += ", ";

				result += players[i];
			}
			sender->sendTranslation("commands.whitelist.list", { SMUtil::toString(players.size()), SMUtil::toString(players.size()) });
			sender->sendMessage(result);
			return true;
		}
	}
	else if ((int)args.size() == 2)
	{
		if (!args[0].compare("add"))
		{
			if (badPerm(sender, "add")) return true;

			ServerManager::getWhitelist()->add(args[1]);
			Command::broadcastCommandTranslation(sender, "commands.whitelist.add.success", { args[1] });
			return true;
		}
		else if (!args[0].compare("remove"))
		{
			if (badPerm(sender, "remove")) return true;

			ServerManager::getWhitelist()->remove(args[1]);
			Command::broadcastCommandTranslation(sender, "commands.whitelist.remove.success", { args[1] });
			return true;
		}
	}
	sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
	return false;
}

bool WhitelistCommand::badPerm(CommandSender *sender, const std::string &perm)
{
	if (!sender->hasPermission("servermanager.command.whitelist." + perm))
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.permission", {});
		return true;
	}
	return false;
}