#pragma once

#include <string>
#include <vector>

class CommandSender;
class Command;

class CommandExecutor
{
public:
	virtual ~CommandExecutor() {};

	virtual bool onCommand(CommandSender *sender, Command *command, std::string &label, std::vector<std::string> &args) = 0;
};
