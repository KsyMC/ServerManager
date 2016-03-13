#pragma once

#include "VanillaCommand.h"

class DeopCommand : public VanillaCommand
{
public:
	DeopCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
