#pragma once

#include "VanillaCommand.h"

class MeCommand : public VanillaCommand
{
public:
	MeCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
