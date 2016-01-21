#include "KickCommand.h"
#include "../../ServerManager.h"
#include "../../entity/SMPlayer.h"
#include "../../util/SMUtil.h"

KickCommand::KickCommand()
	: VanillaCommand("kick")
{
	description = "Removes the specified player from the server";
	usageMessage = "%commands.kick.usage";
}

bool KickCommand::execute(SMPlayer *sender, std::string &label, std::vector<std::string> &args)
{
	if((int)args.size() < 1 || args[0].empty())
	{
		sender->sendTranslation("§c%commands.generic.usage", {usageMessage});
		return false;
	}

	SMPlayer *player = ServerManager::getPlayer(args[0]);
	std::string reason;

	args.erase(args.begin());
	reason = SMUtil::trim(SMUtil::join(args, " "));

	if(!player)
	{
		sender->sendTranslation("§c%commands.generic.player.notFound", {});
		return true;
	}

	if(!reason.empty())
		Command::broadcastCommandTranslation(sender, "commands.kick.success.reason", {player->getName(), reason});
	else
		Command::broadcastCommandTranslation(sender, "commands.kick.success", {player->getName()});

	ServerManager::kickPlayer(player, reason);

	return true;
}
