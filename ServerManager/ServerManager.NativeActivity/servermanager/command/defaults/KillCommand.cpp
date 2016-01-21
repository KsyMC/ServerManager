#include "KillCommand.h"
#include "../../ServerManager.h"
#include "../../entity/SMPlayer.h"
#include "minecraftpe/entity/EntityDamageSource.h"
#include "minecraftpe/entity/player/Player.h"

KillCommand::KillCommand()
	: VanillaCommand("kill")
{
	description = "Commit suicide or kill other players";
	usageMessage = "/kill";
}

bool KillCommand::execute(SMPlayer *sender, std::string &label, std::vector<std::string> &args)
{
	sender->getHandle()->die(EntityDamageSource(CAUSE_SUICIDE));
	sender->sendMessage("Ouch. That look like it hurt.");

	return true;
}
