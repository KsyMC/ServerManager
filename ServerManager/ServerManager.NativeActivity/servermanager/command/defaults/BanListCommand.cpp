#include "BanListCommand.h"
#include "../../ServerManager.h"
#include "../../BanList.h"
#include "../../BanEntry.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

BanListCommand::BanListCommand()
	: VanillaCommand("banlist")
{
	description = "View all players banned from this server";
	usageMessage = "%commands.banlist.usage";
	setPermission("servermanager.command.ban.list");
}

bool BanListCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	BanList::Type banType = BanList::NAME;
	if (args.size() > 0)
	{
		if (!args[0].compare("ips"))
			banType = BanList::IP;
		else if (args[0].compare("players"))
		{
			sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
			return false;
		}
	}

	std::string message;
	std::vector<BanEntry *> banList = ServerManager::getBanList(banType)->getBanEntries();

	for (size_t i = 0; i < banList.size(); i++)
	{
		if (i != 0)
		{
			if (i == banList.size() - 1)
				message += " and ";
			else
				message += ", ";
		}

		message += banList[i]->getTarget();
	}

	if (banType == BanList::IP)
		sender->sendTranslation("commands.banlist.ips", { SMUtil::toString(banList.size()) });
	else if (banType == BanList::NAME)
		sender->sendTranslation("commands.banlist.players", { SMUtil::toString(banList.size()) });

	sender->sendMessage(message);

	return true;
}
