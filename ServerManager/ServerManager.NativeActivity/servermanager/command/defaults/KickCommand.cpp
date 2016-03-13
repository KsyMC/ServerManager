#include "KickCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

KickCommand::KickCommand()
	: VanillaCommand("kick")
{
	description = "Removes the specified player from the server";
	usageMessage = "%commands.kick.usage";
	setPermission("servermanager.command.kick");
}

bool KickCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	if ((int)args.size() < 1 || args[0].empty())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	SMPlayer *player = ServerManager::getPlayer(args[0]);
	if (!player)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.player.notFound", {});
		return true;
	}

	std::string reason;

	if (!reason.empty())
		Command::broadcastCommandTranslation(sender, "commands.kick.success.reason", { player->getName(), reason });
	else
		Command::broadcastCommandTranslation(sender, "commands.kick.success", { player->getName() });

	if ((int)args.size() > 1)
	{
		args.erase(args.begin());
		reason = SMUtil::trim(SMUtil::join(args, " "));
	}
	else
		reason = "Kicked by an operator.";

	player->kick(reason);

	return true;
}
