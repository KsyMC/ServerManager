#include "SMAgableMob.h"
#include "minecraftpe/world/entity/AgableMob.h"

SMAgableMob::SMAgableMob(Server *server, AgableMob *entity)
	: SMPathfinderMob(server, entity)
{
}

AgableMob *SMAgableMob::getHandle() const
{
	return (AgableMob *)entity;
}

void SMAgableMob::setHandle(AgableMob *entity)
{
	this->entity = entity;
}
