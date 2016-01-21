#pragma once

#include "VanillaCommand.h"

class DeopCommand : public VanillaCommand
{
public:
	DeopCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
};
