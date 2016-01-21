#pragma once

#include "PluginEvent.h"

class PluginDisableEvent : public PluginEvent
{
private:
	static HandlerList *handlers;

public:
	PluginDisableEvent(Plugin *plugin);

	HandlerList *getHandlers() const;
	static HandlerList *getHandlerList();
};
