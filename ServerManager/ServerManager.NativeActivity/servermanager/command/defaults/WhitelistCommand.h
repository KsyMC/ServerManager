#pragma once

#include "VanillaCommand.h"

class WhitelistCommand : public VanillaCommand
{
public:
	WhitelistCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
	
private:
	bool badPerm(CommandSender *sender, const std::string &perm);
};
