#include "PermissionAttachment.h"
#include "Permission.h"
#include "Permissible.h"
#include "../util/SMUtil.h"

PermissionAttachment::PermissionAttachment(Plugin *plugin, Permissible *permissible)
{
	this->permissible = permissible;
	this->plugin = plugin;
}

Plugin *PermissionAttachment::getPlugin() const
{
	return plugin;
}

void PermissionAttachment::setRemovalCallback(PermissionRemovedExecutor *ex)
{
	removed = ex;
}

PermissionRemovedExecutor *PermissionAttachment::getRemovalCallback() const
{
	return removed;
}

Permissible *PermissionAttachment::getPermissible() const
{
	return permissible;
}

const std::map<std::string, bool> &PermissionAttachment::getPermissions() const
{
	return permissions;
}

void PermissionAttachment::setPermission(const std::string &name, bool value)
{
	permissions[SMUtil::toLower(name)] = value;
	permissible->recalculatePermissions();
}

void PermissionAttachment::setPermission(Permission *perm, bool value)
{
	setPermission(perm->getName(), value);
}

void PermissionAttachment::unsetPermission(const std::string &name)
{
	permissions.erase(SMUtil::toLower(name));
	permissible->recalculatePermissions();
}

void PermissionAttachment::unsetPermission(Permission *perm)
{
	unsetPermission(perm->getName());
}

bool PermissionAttachment::remove()
{
	permissible->removeAttachment(this);
	return true;
}