#pragma once

#include "VanillaCommand.h"

class GameModeCommand : public VanillaCommand
{
public:
	GameModeCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
