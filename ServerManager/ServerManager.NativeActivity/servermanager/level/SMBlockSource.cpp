#include "SMBlockSource.h"
#include "../entity/SMEntity.h"
#include "minecraftpe/world/entity/item/ItemEntity.h"
#include "minecraftpe/world/level/BlockSource.h"
#include "minecraftpe/world/level/Level.h"

SMBlockSource::SMBlockSource(SMEntity *entity)
{
	this->entity = entity;
}

void SMBlockSource::dropItem(const Vec3 &pos, const ItemInstance &item, int pickupDelay)
{
	ItemEntity *itemEntity = new ItemEntity(*getHandle(), pos, item, pickupDelay);
	getHandle()->getLevel()->addEntity(std::unique_ptr<Entity>(itemEntity));
}

BlockSource *SMBlockSource::getHandle() const
{
	return entity->getHandle()->getRegion();
}
