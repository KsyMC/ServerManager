#pragma once

#include "VanillaCommand.h"

class BanListCommand : public VanillaCommand
{
public:
	BanListCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
