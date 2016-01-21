#pragma once

#include "VanillaCommand.h"

class OpCommand : public VanillaCommand
{
public:
	OpCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
};
