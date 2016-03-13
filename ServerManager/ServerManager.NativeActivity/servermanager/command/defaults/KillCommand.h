#pragma once

#include "VanillaCommand.h"

class KillCommand : public VanillaCommand
{
public:
	KillCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
