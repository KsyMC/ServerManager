#pragma once

#include "PlayerEvent.h"
#include "../Cancellable.h"

class ItemInstance;

class PlayerDropItemEvent : public PlayerEvent, public Cancellable
{
private:
	static HandlerList *handlers;
	ItemInstance *drop;
	bool cancel;

public:
	PlayerDropItemEvent(SMPlayer *player, ItemInstance *drop);

	ItemInstance *getItemDrop() const;

	bool isCancelled() const;
	void setCancelled(bool cancel);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
