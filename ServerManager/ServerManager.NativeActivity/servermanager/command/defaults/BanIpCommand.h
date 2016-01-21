#pragma once

#include "VanillaCommand.h"

class BanIpCommand : public VanillaCommand
{
public:
	BanIpCommand();

	bool execute(SMPlayer *player, std::string &label, std::vector<std::string> &args);
	void processIPBan(const std::string &ip, SMPlayer *source, const std::string &reason);
};
