#include "PlayerBedEnterEvent.h"
#include "../HandlerList.h"

HandlerList *PlayerBedEnterEvent::handlers = new HandlerList;

PlayerBedEnterEvent::PlayerBedEnterEvent(SMPlayer *who, FullBlock bed)
	: PlayerEvent(who)
{
	this->bed = bed;
	cancel = false;
}

FullBlock PlayerBedEnterEvent::getBed() const
{
	return bed;
}

bool PlayerBedEnterEvent::isCancelled() const
{
	return cancel;
}

void PlayerBedEnterEvent::setCancelled(bool cancel)
{
	this->cancel = cancel;
}

HandlerList *PlayerBedEnterEvent::getHandlers() const
{
	return handlers;
}

HandlerList *PlayerBedEnterEvent::getHandlerList()
{
	return handlers;
}
