#pragma once

#include "VanillaCommand.h"

class PardonIpCommand : public VanillaCommand
{
public:
	PardonIpCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
