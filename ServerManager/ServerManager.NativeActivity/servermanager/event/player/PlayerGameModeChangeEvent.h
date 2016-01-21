#pragma once

#include "PlayerEvent.h"
#include "../Cancellable.h"
#include "minecraftpe/gamemode/GameType.h"

class PlayerGameModeChangeEvent : public PlayerEvent, public Cancellable
{
private:
	static HandlerList *handlers;
	GameType gametype;
	bool cancel;

public:
	PlayerGameModeChangeEvent(SMPlayer *player, GameType newGameType);

	GameType getNewGameType();

	bool isCancelled() const;
	void setCancelled(bool cancel);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
