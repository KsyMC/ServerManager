#pragma once

#include <string>
#include <vector>

#include "ServerOperator.h"

class Permission;
class PermissionAttachment;
class PermissionAttachmentInfo;
class Plugin;

class Permissible : public ServerOperator
{
public:
	virtual bool isPermissionSet(const std::string &name) = 0;
	virtual bool isPermissionSet(Permission *perm) = 0;
	virtual bool hasPermission(const std::string &name) = 0;
	virtual bool hasPermission(Permission *perm) = 0;

	virtual PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value) = 0;
	virtual PermissionAttachment *addAttachment(Plugin *plugin) = 0;
	virtual PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value, int ticks) = 0;
	virtual PermissionAttachment *addAttachment(Plugin *plugin, int ticks) = 0;

	virtual void removeAttachment(PermissionAttachment *attachment) = 0;
	virtual void recalculatePermissions() = 0;

	virtual std::vector<PermissionAttachmentInfo *> getEffectivePermissions() const = 0;

	virtual bool isPermissible() { return true; }
	virtual bool isCommandSender() { return false; }
};