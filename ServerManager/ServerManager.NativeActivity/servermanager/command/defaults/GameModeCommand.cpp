#include "GameModeCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"

GameModeCommand::GameModeCommand()
	: VanillaCommand("gamemode")
{
	description = "Changes the player to a specific game mode";
	usageMessage = "%commands.gamemode.usage";
	setPermission("servermanager.command.gamemode");
}

bool GameModeCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if (args.empty())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	std::string modeArg = args[0];
	std::string playerArg = sender->getName();

	if ((int)args.size() >= 2)
		playerArg = args[1];

	SMPlayer *player = ServerManager::getPlayer(playerArg);
	if (!player)
	{
		sender->sendTranslation("commands.generic.player.notFound", {});
		return false;
	}

	GameType gametype = Server::getGamemodeFromString(args[0]);
	if (gametype == GameType::UNDEFINED)
	{
		sender->sendMessage("Unknown game mode");
		return true;
	}

	if (player->getGameType() == gametype)
	{
		sender->sendMessage(player->getName() + " already has game mode " + SMUtil::toString((int)gametype));
		return true;
	}
	player->setGameType(gametype);

	if (gametype != player->getGameType())
		sender->sendMessage("Game mode change for " + player->getName() + " failed!");
	else
	{
		if (sender == player)
			Command::broadcastCommandTranslation(sender, "commands.gamemode.success.self", { Server::getGamemodeString(gametype) });
		else
			Command::broadcastCommandTranslation(sender, "commands.gamemode.success.other", { player->getName(), Server::getGamemodeString(gametype) });
	}
	return true;
}
