#pragma once

#include "VanillaCommand.h"

class TeleportCommand : public VanillaCommand
{
public:
	TeleportCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);

private:
	double getCoordinate(CommandSender *sender, double current, std::string &input, int max = MAX_COORD, int min = MIN_COORD);
};
