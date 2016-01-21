#pragma once

#include "VanillaCommand.h"

class KillCommand : public VanillaCommand
{
public:
	KillCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
};
