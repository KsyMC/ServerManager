#include "KillCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "minecraftpe/world/entity/EntityDamageSource.h"
#include "minecraftpe/world/entity/player/Player.h"

KillCommand::KillCommand()
	: VanillaCommand("kill")
{
	description = "Commit suicide or kill other players";
	usageMessage = "/kill";
	setPermission("servermanager.command.kill");
}

bool KillCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;

	SMPlayer *player = (SMPlayer *)sender;
	player->getHandle()->die(EntityDamageSource(CAUSE_SUICIDE));
	sender->sendMessage("Ouch. That look like it hurt.");

	return true;
}
