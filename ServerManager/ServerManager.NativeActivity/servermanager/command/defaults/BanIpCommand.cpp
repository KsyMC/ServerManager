#include <regex>

#include "BanIpCommand.h"
#include "../../ServerManager.h"
#include "../../BanList.h"
#include "../../entity/SMPlayer.h"
#include "../../util/SMUtil.h"

BanIpCommand::BanIpCommand()
	: VanillaCommand("ban-ip")
{
	description = "Prevents the specified IP address from using this server",
		usageMessage = "%commands.banip.usage";
}

bool BanIpCommand::execute(SMPlayer *sender, std::string &label, std::vector<std::string> &args)
{
	if ((int)args.size() < 1)
	{
		sender->sendTranslation("§c%commands.generic.usage", { usageMessage });
		return false;
	}

	std::string nameOrIP = args[0];

	args.erase(args.begin());
	std::string reason = SMUtil::trim(SMUtil::join(args, " "));

	std::regex rx("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	if (std::regex_match(nameOrIP, rx))
	{
		Command::broadcastCommandTranslation(sender, "commands.banip.success", { nameOrIP });
		processIPBan(nameOrIP, sender, reason);
	}
	else
	{
		SMPlayer *player = ServerManager::getPlayer(nameOrIP);
		if (!player)
		{
			sender->sendTranslation("§c%commands.banip.invalid", {});
			return false;
		}

		std::string ip = player->getAddress();

		Command::broadcastCommandTranslation(sender, "commands.banip.success.players", { ip, player->getName() });
		processIPBan(ip, sender, reason);
	}
	return true;
}

void BanIpCommand::processIPBan(const std::string &ip, SMPlayer *source, const std::string &reason)
{
	ServerManager::getBanList(BanList::IP)->addBan(ip, reason, source->getName());

	std::vector<SMPlayer *> players = ServerManager::getOnlinePlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		SMPlayer *player = players[i];
		if (!ip.compare(player->getAddress()))
			ServerManager::kickPlayer(player, "You have been IP banned.");
	}
}
