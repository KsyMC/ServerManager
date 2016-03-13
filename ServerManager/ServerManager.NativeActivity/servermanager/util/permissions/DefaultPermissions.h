#pragma once

#include <string>
#include <map>

#include "../../permissions/PermissionDefault.h"

class Permission;

class DefaultPermissions
{
private:
	static const std::string ROOT;
	static const std::string LEGACY_PREFIX;

public:
	static Permission *registerPermission(Permission *perm, bool withLegacy = true);
	static Permission *registerPermission(Permission *perm, Permission *parent);
	static Permission *registerPermission(const std::string &name, const std::string &desc);
	static Permission *registerPermission(const std::string &name, const std::string &desc, Permission *parent);
	static Permission *registerPermission(const std::string &name, const std::string &desc, PermissionDefault def);
	static Permission *registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, Permission *parent);
	static Permission *registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, std::map<std::string, bool> children);
	static Permission *registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, std::map<std::string, bool> children, Permission *parent);

	static void registerCorePermissions();
};