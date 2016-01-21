#pragma once

#include "VanillaCommand.h"

class TimeCommand : public VanillaCommand
{
public:
	TimeCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
};
