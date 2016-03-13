#pragma once

#include "VanillaCommand.h"

class PardonCommand : public VanillaCommand
{
public:
	PardonCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
