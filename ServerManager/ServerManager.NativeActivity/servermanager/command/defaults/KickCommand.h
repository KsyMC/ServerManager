#pragma once

#include "VanillaCommand.h"

class KickCommand : public VanillaCommand
{
public:
	KickCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
