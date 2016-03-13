#include "SMMonster.h"
#include "minecraftpe/world/entity/monster/Monster.h"

SMMonster::SMMonster(Server *server, Monster *entity)
	: SMPathfinderMob(server, entity)
{
}

Monster *SMMonster::getHandle() const
{
	return (Monster *)entity;
}

void SMMonster::setHandle(Monster *entity)
{
	this->entity = entity;
}
