#pragma once

#include "PlayerEvent.h"
#include "../Cancellable.h"

class PlayerKickEvent : public PlayerEvent, public Cancellable
{
private:
	static HandlerList *handlers;
	std::string leaveMessage;
	std::string kickReason;
	bool cancel;

public:
	PlayerKickEvent(SMPlayer *playerKicked, const std::string &kickReason, const std::string &leaveMessage);

	const std::string &getReason() const;
	const std::string &getLeaveMessage() const;

	void setReason(const std::string &reason);
	void setLeaveMessage(const std::string &leaveMessage);

	bool isCancelled() const;
	void setCancelled(bool cancel);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};

