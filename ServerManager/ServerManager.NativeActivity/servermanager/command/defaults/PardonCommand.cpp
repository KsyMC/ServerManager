#include "PardonCommand.h"
#include "../../ServerManager.h"
#include "../../BanList.h"
#include "../../entity/player/SMPlayer.h"
#include "../../ChatColor.h"

PardonCommand::PardonCommand()
	: VanillaCommand("pardon")
{
	description = "Allows the specified player to use this server";
	usageMessage = "%commands.unban.usage";
	setPermission("servermanager.command.unban.player");
}

bool PardonCommand::execute(CommandSender *sender, std::string &commandLabel, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if ((int)args.size() != 1)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	ServerManager::getBanList(BanList::NAME)->pardon(args[0]);
	Command::broadcastCommandTranslation(sender, "commands.unban.success", { args[0] });

	return true;
}
