#include "BanCommand.h"
#include "../../ServerManager.h"
#include "../../BanList.h"
#include "../../entity/player/SMPlayer.h"
#include "../../level/SMLevel.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

BanCommand::BanCommand()
	: VanillaCommand("ban")
{
	description = "Prevents the specified player from using this server";
	usageMessage = "%commands.ban.usage";
	setPermission("servermanager.command.ban.player");
}

bool BanCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if (args.empty())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	std::string name = args[0];

	args.erase(args.begin());
	std::string reason = SMUtil::trim(SMUtil::join(args, " "));

	ServerManager::getBanList(BanList::NAME)->addBan(name, reason, sender->getName());

	Command::broadcastCommandTranslation(sender, "commands.ban.success", { name });

	SMPlayer *player = ServerManager::getPlayerExact(name);
	if (player)
		player->kick("Banned by admin.");

	return true;
}
