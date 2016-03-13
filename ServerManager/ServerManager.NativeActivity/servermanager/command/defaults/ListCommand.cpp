#include "ListCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"

ListCommand::ListCommand()
	: VanillaCommand("list")
{
	description = "Lists all online players";
	usageMessage = "%commands.players.usage";
	setPermission("servermanager.command.list");
}

bool ListCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	std::string online;

	std::vector<SMPlayer *> players = ServerManager::getOnlinePlayers();
	for (size_t i = 0; i < players.size(); i++)
	{
		if (i > 0)
			online += ", ";

		SMPlayer *player = players[i];
		online += player->getDisplayName();
	}
	sender->sendTranslation("commands.players.list", { SMUtil::toString(players.size()), SMUtil::toString(ServerManager::getMaxPlayers()) });
	sender->sendMessage(online);

	return true;
}
