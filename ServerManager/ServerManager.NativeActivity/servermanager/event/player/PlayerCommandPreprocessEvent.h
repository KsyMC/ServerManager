#pragma once

#include "PlayerEvent.h"
#include "../Cancellable.h"

class PlayerCommandPreprocessEvent : public PlayerEvent, public Cancellable
{
private:
	static HandlerList *handlers;
	std::string message;
	bool cancel;

public:
	PlayerCommandPreprocessEvent(SMPlayer *player, const std::string &message);

	const std::string &getMessage() const;
	void setMessage(const std::string &command);

	void setPlayer(SMPlayer *player);

	bool isCancelled() const;
	void setCancelled(bool cancel);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
