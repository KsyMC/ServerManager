#pragma once

#include "VanillaCommand.h"

class ListCommand : public VanillaCommand
{
public:
	ListCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
};
