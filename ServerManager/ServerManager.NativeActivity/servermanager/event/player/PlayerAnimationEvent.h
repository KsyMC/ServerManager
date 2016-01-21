#pragma once

#include "PlayerEvent.h"
#include "../Cancellable.h"
#include "PlayerAnimationType.h"

class PlayerAnimationEvent : public PlayerEvent, public Cancellable
{
private:
	static HandlerList *handlers;
	PlayerAnimationType animationType;
	bool cancel;

public:
	PlayerAnimationEvent(SMPlayer *player, PlayerAnimationType animationType);

	PlayerAnimationType getAnimationType() const;

	bool isCancelled() const;
	void setCancelled(bool cancel);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
