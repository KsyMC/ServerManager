#include "SMItemEntity.h"
#include "minecraftpe/world/entity/item/ItemEntity.h"

SMItemEntity::SMItemEntity(Server *server, ItemEntity *entity)
	: SMEntity(server, entity)
{
}

ItemEntity *SMItemEntity::getHandle() const
{
	return (ItemEntity *)entity;
}

void SMItemEntity::setHandle(ItemEntity *entity)
{
	this->entity = entity;
}
