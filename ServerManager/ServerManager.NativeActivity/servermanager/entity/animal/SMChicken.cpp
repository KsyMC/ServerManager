#include "SMChicken.h"
#include "minecraftpe/world/entity/animal/Chicken.h"

SMChicken::SMChicken(Server *server, Chicken *entity)
	: SMAnimal(server, entity)
{
}

Chicken *SMChicken::getHandle() const
{
	return (Chicken *)entity;
}

void SMChicken::setHandle(Chicken *entity)
{
	this->entity = entity;
}
