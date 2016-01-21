#pragma once

#include "VanillaCommand.h"

class PardonIpCommand : public VanillaCommand
{
public:
	PardonIpCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
};
