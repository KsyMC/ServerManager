#pragma once

#include <vector>

class Command;
class Plugin;

class PluginCommandJsonParser
{
public:
	static std::vector<Command *> parse(Plugin *plugin);
};