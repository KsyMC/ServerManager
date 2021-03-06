#include "SMEnderman.h"
#include "minecraftpe/world/entity/monster/Enderman.h"

SMEnderman::SMEnderman(Server *server, Enderman *entity)
	: SMMob(server, entity)
{
}

Enderman *SMEnderman::getHandle() const
{
	return (Enderman *)entity;
}

void SMEnderman::setHandle(Enderman *entity)
{
	this->entity = entity;
}
