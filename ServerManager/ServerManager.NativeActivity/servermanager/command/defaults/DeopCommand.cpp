#include "DeopCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../ChatColor.h"

DeopCommand::DeopCommand()
	: VanillaCommand("deop")
{
	description = "Takes the specified player's operator status";
	usageMessage = "%commands.deop.usage";
	setPermission("servermanager.command.op.take");
}

bool DeopCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if ((int)args.size() != 1 || args[0].empty())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	SMPlayer *player = ServerManager::getPlayerExact(args[0]);
	if (player)
	{
		player->setOp(false);
		player->sendMessage(ChatColor::GRAY + "You are no longer op!");
	}
	else
		ServerManager::getServer()->removeOp(args[0]);

	Command::broadcastCommandTranslation(sender, "commands.deop.success", { args[0] });

	return true;
}
