#include "SMZombie.h"
#include "minecraftpe/world/entity/monster/Zombie.h"

SMZombie::SMZombie(Server *server, Zombie *entity)
	: SMMonster(server, entity)
{
}

Zombie *SMZombie::getHandle() const
{
	return (Zombie *)entity;
}

void SMZombie::setHandle(Zombie *entity)
{
	this->entity = entity;
}
