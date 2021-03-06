#include "SMCreeper.h"
#include "../../Server.h"
#include "../../event/entity/CreeperPowerEvent.h"
#include "../../plugin/PluginManager.h"
#include "minecraftpe/world/entity/monster/Creeper.h"

SMCreeper::SMCreeper(Server *server, Creeper *entity)
	: SMMob(server, entity)
{
}

bool SMCreeper::isPowered() const
{
	return getHandle()->isPowered();
}

void SMCreeper::setPowered(bool powered)
{
	CreeperPowerEvent event(this, powered ? CreeperPowerEvent::SET_ON : CreeperPowerEvent::SET_OFF);
	server->getPluginManager()->callEvent(event);

	if(!event.isCancelled())
		getHandle()->_setPowered(powered);
}

Creeper *SMCreeper::getHandle() const
{
	return (Creeper *)entity;
}

void SMCreeper::setHandle(Creeper *entity)
{
	this->entity = entity;
}
