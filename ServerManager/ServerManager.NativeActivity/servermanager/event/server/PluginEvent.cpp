#include "PluginEvent.h"

PluginEvent::PluginEvent(Plugin *plugin)
{
	this->plugin = plugin;
}

Plugin *PluginEvent::getPlugin() const
{
	return plugin;
}
