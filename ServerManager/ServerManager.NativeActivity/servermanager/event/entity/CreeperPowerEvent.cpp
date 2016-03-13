#include "CreeperPowerEvent.h"
#include "../../entity/monster/SMCreeper.h"
#include "../HandlerList.h"

HandlerList *CreeperPowerEvent::handlers = new HandlerList;

CreeperPowerEvent::CreeperPowerEvent(SMCreeper *creeper, PowerCause cause)
	: SMEntityEvent(creeper)
{
	this->cause = cause;
	cancel = false;
}

SMCreeper *CreeperPowerEvent::getEntity() const
{
	return (SMCreeper *)entity;
}

bool CreeperPowerEvent::isCancelled() const
{
	return cancel;
}

void CreeperPowerEvent::setCancelled(bool cancel)
{
	this->cancel = cancel;
}

HandlerList *CreeperPowerEvent::getHandlers() const
{
	return handlers;
}

HandlerList *CreeperPowerEvent::getHandlerList()
{
	return handlers;
}
