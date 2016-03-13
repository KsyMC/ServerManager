#include "TellCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

TellCommand::TellCommand()
	: VanillaCommand("tell")
{
	description = "Sends a private message to the given player";
	usageMessage = "%commands.message.usage";
	setAliases({ "w", "msg" });
	setPermission("servermanager.command.tell");
}

bool TellCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	if ((int)args.size() < 2)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	SMPlayer *player = ServerManager::getPlayer(args[0]);
	if (sender == player)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.message.sameTarget", {});
		return true;
	}

	if (!player)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.player.notFound", {});
		return true;
	}

	args.erase(args.begin());
	std::string message = SMUtil::join(args, " ");

	sender->sendTranslation("commands.message.display.outgoing", { player->getName(), message });
	player->sendTranslation("commands.message.display.incoming", { sender->getName(), message });

	return true;
}
