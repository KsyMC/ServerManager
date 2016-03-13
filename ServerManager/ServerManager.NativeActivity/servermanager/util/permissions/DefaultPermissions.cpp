#include "DefaultPermissions.h"
#include "../../ServerManager.h"
#include "../../permissions/Permission.h"
#include "../../plugin/PluginManager.h"
#include "CommandPermissions.h"
#include "BroadcastPermissions.h"

const std::string DefaultPermissions::ROOT = "servermanager";
const std::string DefaultPermissions::LEGACY_PREFIX = "sm";

Permission *DefaultPermissions::registerPermission(Permission *perm, bool withLegacy)
{
	Permission *result = perm;

	if (!ServerManager::getPluginManager()->addPermission(perm))
		result = ServerManager::getPluginManager()->getPermission(perm->getName());

	if (withLegacy)
	{
		Permission *legacy = new Permission(LEGACY_PREFIX + result->getName(), result->getDescription(), PermissionDefault::FALSE);
		legacy->getChildren()[result->getName()] = true;
		registerPermission(perm, false);
	}
	return result;
}

Permission *DefaultPermissions::registerPermission(Permission *perm, Permission *parent)
{
	parent->getChildren()[perm->getName()] = true;
	return registerPermission(perm);
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc)
{
	return registerPermission(new Permission(name, desc));
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc, Permission *parent)
{
	Permission *perm = registerPermission(name, desc);
	parent->getChildren()[perm->getName()] = true;
	return perm;
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc, PermissionDefault def)
{
	return registerPermission(new Permission(name, desc, def));
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, Permission *parent)
{
	Permission *perm = registerPermission(name, desc, def);
	parent->getChildren()[perm->getName()] = true;
	return perm;
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, std::map<std::string, bool> children)
{
	return registerPermission(new Permission(name, desc, def, children));
}

Permission *DefaultPermissions::registerPermission(const std::string &name, const std::string &desc, PermissionDefault def, std::map<std::string, bool> children, Permission *parent)
{
	Permission *perm = registerPermission(new Permission(name, desc, def, children));
	parent->getChildren()[perm->getName()] = true;
	return perm;
}

void DefaultPermissions::registerCorePermissions()
{
	Permission *parent = registerPermission(ROOT, "Gives the user the ability to use all ServerManager utilities and commands");
	CommandPermissions::registerPermissions(parent);
	BroadcastPermissions::registerPermissions(parent);

	parent->recalculatePermissibles();
}