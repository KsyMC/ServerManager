#pragma once

#include "VanillaCommand.h"

class TellCommand : public VanillaCommand
{
public:
	TellCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
