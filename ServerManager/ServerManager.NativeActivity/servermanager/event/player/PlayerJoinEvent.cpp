#include "PlayerJoinEvent.h"
#include "../HandlerList.h"

HandlerList *PlayerJoinEvent::handlers = new HandlerList;

PlayerJoinEvent::PlayerJoinEvent(SMPlayer *playerJoined, const std::string &joinMessage)
	: PlayerEvent(playerJoined)
{
	this->joinMessage = joinMessage;
}

const std::string &PlayerJoinEvent::getJoinMessage() const
{
	return joinMessage;
}

// 작동 안함
void PlayerJoinEvent::setJoinMessage(const std::string &joinMessage)
{
	this->joinMessage = joinMessage;
}

HandlerList *PlayerJoinEvent::getHandlers() const
{
	return handlers;
}

HandlerList *PlayerJoinEvent::getHandlerList()
{
	return handlers;
}
