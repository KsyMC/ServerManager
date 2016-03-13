#include "MeCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

MeCommand::MeCommand()
	: VanillaCommand("me")
{
	description = "Performs the specified action in chat";
	usageMessage = "%commands.me.usage";
	setPermission("servermanager.command.me");
}

bool MeCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	if ((int)args.size() < 1)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}
	ServerManager::broadcastMessage("* " + sender->getName() + " " + SMUtil::join(args, " "));

	return true;
}
