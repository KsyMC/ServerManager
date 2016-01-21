#include "SMEntityEvent.h"
#include "../../entity/SMEntity.h"

SMEntityEvent::SMEntityEvent(SMEntity *what)
{
	entity = what;
}

SMEntity *SMEntityEvent::getEntity() const
{
	return entity;
}

EntityType SMEntityEvent::getEntityType() const
{
	return entity->getEntityTypeId();
}
