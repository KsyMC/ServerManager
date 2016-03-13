#pragma once

#include "VanillaCommand.h"

class ToogleDownFallCommand : public VanillaCommand
{
public:
	ToogleDownFallCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
