#pragma once

#include <map>
#include <string>

class Permissible;
class Plugin;
class PermissionRemovedExecutor;
class Permission;

class PermissionAttachment
{
private:
	PermissionRemovedExecutor *removed;
	std::map<std::string, bool> permissions;
	Permissible *permissible;
	Plugin *plugin;

public:
	PermissionAttachment(Plugin *plugin, Permissible *permissible);

	Plugin *getPlugin() const;

	void setRemovalCallback(PermissionRemovedExecutor *ex);
	PermissionRemovedExecutor *getRemovalCallback() const;

	Permissible *getPermissible() const;

	const std::map<std::string, bool> &getPermissions() const;
	void setPermission(const std::string &name, bool value);
	void setPermission(Permission *perm, bool value);
	void unsetPermission(const std::string &name);
	void unsetPermission(Permission *perm);

	bool remove();
};