#include "SMBoat.h"
#include "minecraftpe/world/entity/item/Boat.h"

SMBoat::SMBoat(Server *server, Boat *entity)
	: SMEntity(server, entity)
{
}

Boat *SMBoat::getHandle() const
{
	return (Boat *)entity;
}

void SMBoat::setHandle(Boat *entity)
{
	this->entity = entity;
}
