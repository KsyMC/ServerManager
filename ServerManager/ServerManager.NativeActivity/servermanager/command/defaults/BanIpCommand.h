#pragma once

#include "VanillaCommand.h"

class BanIpCommand : public VanillaCommand
{
public:
	BanIpCommand();

	bool execute(CommandSender *sender, std::string &label, std::vector<std::string> &args);
	void processIPBan(const std::string &ip, CommandSender *source, const std::string &reason);
};
