#include "SMMushroomCow.h"
#include "minecraftpe/world/entity/animal/Mushroomcow.h"

SMMushroomCow::SMMushroomCow(Server *server, MushroomCow *entity)
	: SMAnimal(server, entity)
{
}

MushroomCow *SMMushroomCow::getHandle() const
{
	return (MushroomCow *)entity;
}

void SMMushroomCow::setHandle(MushroomCow *entity)
{
	this->entity = entity;
}
