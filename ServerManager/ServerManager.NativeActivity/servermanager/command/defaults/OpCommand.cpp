#include "OpCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../ChatColor.h"

OpCommand::OpCommand()
	: VanillaCommand("op")
{
	description = "Gives the specified player's operator status";
	usageMessage = "%commands.op.usage";
	setPermission("servermanager.command.op.give");
}

bool OpCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	if((int)args.size() != 1 || args[0].empty())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", {usageMessage});
		return false;
	}

	SMPlayer *player = ServerManager::getPlayerExact(args[0]);
	if (player)
	{
		player->setOp(true);
		player->sendMessage(ChatColor::GRAY + "You are now op!");
	}
	else
		ServerManager::getServer()->addOp(args[0]);

	Command::broadcastCommandTranslation(sender, "commands.op.success", {args[0]});

	return true;
}
