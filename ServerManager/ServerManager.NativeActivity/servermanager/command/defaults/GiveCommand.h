#pragma once

#include "VanillaCommand.h"

class GiveCommand : public VanillaCommand
{
public:
	GiveCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
