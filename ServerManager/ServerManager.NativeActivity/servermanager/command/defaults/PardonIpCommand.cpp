#include <regex>

#include "PardonIpCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../ChatColor.h"

PardonIpCommand::PardonIpCommand()
	: VanillaCommand("pardon-ip")
{
	description = "Allows the specified IP address to use this server";
	usageMessage = "%commands.unbanip.usage";
	setPermission("servermanager.command.unban.ip");
}

bool PardonIpCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if ((int)args.size() != 1)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	std::regex rx("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	if (std::regex_match(args[0], rx))
	{
		ServerManager::unbanIP(args[0]);
		Command::broadcastCommandTranslation(sender, "commands.unbanip.success", { args[0] });
	}
	else
		sender->sendTranslation("commands.unbanip.invalid", {});

	return true;
}
