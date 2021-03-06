#include "SMWaterMob.h"
#include "minecraftpe/world/entity/WaterAnimal.h"

SMWaterMob::SMWaterMob(Server *server, WaterAnimal *entity)
	: SMAgableMob(server, entity)
{
}

WaterAnimal *SMWaterMob::getHandle() const
{
	return (WaterAnimal *)entity;
}

void SMWaterMob::setHandle(WaterAnimal *entity)
{
	this->entity = entity;
}
