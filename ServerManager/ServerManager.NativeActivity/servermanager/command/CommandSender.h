#pragma once

#include "../permissions/Permissible.h"

class Server;

class CommandSender : public Permissible
{
public:
	virtual void sendMessage(const std::string &message) = 0;
	virtual void sendMessage(const std::vector<std::string> &messages) = 0;
	virtual void sendTranslation(const std::string &message, const std::vector<std::string> &params) = 0;

	virtual Server *getServer() const = 0;
	virtual const std::string &getName() const = 0;

	virtual bool isCommandSender() { return true; }
};