#pragma once

#include "VanillaCommand.h"

class BanCommand : public VanillaCommand
{
public:
	BanCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
