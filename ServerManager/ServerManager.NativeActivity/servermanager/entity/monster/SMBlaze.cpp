#include "SMBlaze.h"
#include "minecraftpe/world/entity/monster/Blaze.h"

SMBlaze::SMBlaze(Server *server, Blaze *entity)
	: SMMob(server, entity)
{
}

Blaze *SMBlaze::getHandle() const
{
	return (Blaze *)entity;
}

void SMBlaze::setHandle(Blaze *entity)
{
	this->entity = entity;
}
