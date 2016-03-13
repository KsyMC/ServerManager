#include <cmath>

#include "HelpCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../CommandMap.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"
#include "minecraftpe/locale/I18n.h"

HelpCommand::HelpCommand()
	: VanillaCommand("help")
{
	description = "Shows the help menu";
	usageMessage = "%commands.help.usage";
	setAliases({ "?" });
	setPermission("servermanager.command.help");
}

bool HelpCommand::execute(CommandSender *sender, std::string &commandLabel, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	std::string command;
	int pageNumber = 1;
	int pageHeight = 5;

	if ((int)args.size() >= 1)
	{
		if (SMUtil::is_number(args[0]))
		{
			pageNumber = getInteger(sender, args[0], 1);
			if (pageNumber < 1)
				pageNumber = 1;
		}
		else
			command = SMUtil::join(args, " ");
	}

	if (command.empty())
	{
		struct comp
		{
			bool operator() (const std::string &left, const std::string &right) const
			{
				return left < right;
			}
		};

		std::map<std::string, Command *, comp> commandList;

		for (auto &pair : ServerManager::getServer()->getCommandMap()->getCommands())
		{
			Command *command = pair.second;
			if (command->testPermissionSilent(sender))
				commandList[command->getName()] = command;
		}
		int pages = (int)ceil((int)commandList.size() / (float)pageHeight);

		std::vector<Command *> sortCommands;
		for (auto &key : commandList)
			sortCommands.push_back(key.second);

		pageNumber = std::min(pages, pageNumber);

		sender->sendTranslation("commands.help.header", { SMUtil::toString(pageNumber), SMUtil::toString(pages) });

		int startIndex = pageHeight * (pageNumber - 1);
		for (int i = startIndex; i < startIndex + pageHeight; i++)
		{
			if ((int)sortCommands.size() <= i)
				break;

			Command *command = sortCommands[i];
			sender->sendTranslation(ChatColor::DARK_GREEN + "#" + command->getName() + ": " + ChatColor::WHITE + command->getDescription(), {});
		}
		return true;
	}
	else
	{
		Command *cmd = ServerManager::getServer()->getCommandMap()->getCommand(command);
		if (cmd)
		{
			std::string message;
			sender->sendTranslation(ChatColor::YELLOW + "--------- " + ChatColor::WHITE + "Help: #" + cmd->getName() + " " + ChatColor::YELLOW + "---------\n", {});
			sender->sendTranslation(ChatColor::GOLD + "Description: " + ChatColor::WHITE + cmd->getDescription() + "\n", {});
			sender->sendTranslation(ChatColor::GOLD + "Usage: " + ChatColor::WHITE + SMUtil::join(SMUtil::split(cmd->getUsage(), '\n'), "\n" + ChatColor::WHITE) + "\n", {});
		}
		else
			sender->sendMessage(ChatColor::RED + "No help for " + SMUtil::toLower(command));

		return true;
	}
}
