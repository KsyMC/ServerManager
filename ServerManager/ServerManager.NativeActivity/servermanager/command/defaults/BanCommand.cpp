#include "BanCommand.h"
#include "../../ServerManager.h"
#include "../../BanList.h"
#include "../../entity/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../level/SMLevel.h"

BanCommand::BanCommand()
	: VanillaCommand("ban")
{
	description = "Prevents the specified player from using this server";
	usageMessage = "%commands.ban.usage";
}

bool BanCommand::execute(SMPlayer *sender, std::string &label, std::vector<std::string> &args)
{
	if(args.empty())
	{
		sender->sendTranslation("§c%commands.generic.usage", {usageMessage});
		return false;
	}

	std::string name = args[0];

	args.erase(args.begin());
	std::string reason = SMUtil::trim(SMUtil::join(args, " "));

	ServerManager::getBanList(BanList::NAME)->addBan(name, reason, sender->getName());

	Command::broadcastCommandTranslation(sender, "commands.ban.success", {name});

	SMPlayer *player = ServerManager::getPlayerExact(name);
	if(player)
		ServerManager::kickPlayer(player, "Banned by admin.");

	return true;
}
