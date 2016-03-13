#pragma once

#include "VanillaCommand.h"

class TimeCommand : public VanillaCommand
{
public:
	TimeCommand();

	bool execute(CommandSender *player, std::string &label, std::vector<std::string> &args);

private:
	bool badPerm(CommandSender *sender, const std::string &perm);
};
