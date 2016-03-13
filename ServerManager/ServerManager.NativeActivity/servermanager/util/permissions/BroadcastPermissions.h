#pragma once

#include <string>

class Permission;

class BroadcastPermissions
{
private:
	static const std::string ROOT;
	static const std::string PREFIX;

public:
	static Permission *registerPermissions(Permission *parent);
};