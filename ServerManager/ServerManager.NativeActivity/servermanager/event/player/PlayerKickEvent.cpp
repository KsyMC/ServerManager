#include "PlayerKickEvent.h"
#include "../HandlerList.h"

HandlerList *PlayerKickEvent::handlers = new HandlerList;

PlayerKickEvent::PlayerKickEvent(SMPlayer *playerKicked, const std::string &kickReason, const std::string &leaveMessage)
	: PlayerEvent(playerKicked)
{
	this->kickReason = kickReason;
	this->leaveMessage = leaveMessage;
	cancel = false;
}

const std::string &PlayerKickEvent::getReason() const
{
	return kickReason;
}

const std::string &PlayerKickEvent::getLeaveMessage() const
{
	return leaveMessage;
}

void PlayerKickEvent::setReason(const std::string &reason)
{
	this->kickReason = reason;
}

void PlayerKickEvent::setLeaveMessage(const std::string &leaveMessage)
{
	this->leaveMessage = leaveMessage;
}

bool PlayerKickEvent::isCancelled() const
{
	return cancel;
}

void PlayerKickEvent::setCancelled(bool cancel)
{
	this->cancel = cancel;
}

HandlerList *PlayerKickEvent::getHandlers() const
{
	return handlers;
}

HandlerList *PlayerKickEvent::getHandlerList()
{
	return handlers;
}
