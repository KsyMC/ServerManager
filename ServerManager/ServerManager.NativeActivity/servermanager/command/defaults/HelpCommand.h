#pragma once

#include "VanillaCommand.h"

class HelpCommand : public VanillaCommand
{
public:
	HelpCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
