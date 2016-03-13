#pragma once

#include <string>

class Permission;

class CommandPermissions
{
private:
	static const std::string ROOT;
	static const std::string PREFIX;

	static Permission *registerWhitelist(Permission *parent);
	static Permission *registerBan(Permission *parent);
	static Permission *registerUnban(Permission *parent);
	static Permission *registerOp(Permission *parent);
	static Permission *registerSave(Permission *parent);
	static Permission *registerTime(Permission *parent);

public:
	static Permission *registerPermissions(Permission *parent);
};