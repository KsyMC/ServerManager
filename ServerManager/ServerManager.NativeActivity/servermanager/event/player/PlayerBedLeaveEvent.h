#pragma once

#include "PlayerEvent.h"
#include "minecraftpe/CommonTypes.h"

class PlayerBedLeaveEvent : public PlayerEvent
{
private:
	static HandlerList *handlers;
	FullBlock bed;

public:
	PlayerBedLeaveEvent(SMPlayer *who, FullBlock bed);

	FullBlock getBed() const;

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
