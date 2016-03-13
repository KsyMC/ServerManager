#include "SMPig.h"
#include "minecraftpe/world/entity/animal/Pig.h"

SMPig::SMPig(Server *server, Pig *entity)
	: SMAnimal(server, entity)
{
}

Pig *SMPig::getHandle() const
{
	return (Pig *)entity;
}

void SMPig::setHandle(Pig *entity)
{
	this->entity = entity;
}
