#pragma once

#include <map>

#include "Permissible.h"

class PermissibleBase : public Permissible
{
private:
	ServerOperator *opable;
	Permissible *parent;
	std::vector<PermissionAttachment *> attachments;
	std::map<std::string, PermissionAttachmentInfo *> permissions;

public:
	PermissibleBase(ServerOperator *opable);

	bool isOp() const;
	void setOp(bool value);

	bool isPermissionSet(const std::string &name);
	bool isPermissionSet(Permission *perm);
	bool hasPermission(const std::string &inName);
	bool hasPermission(Permission *perm);

	PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value);
	PermissionAttachment *addAttachment(Plugin *plugin);
	PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value, int ticks);
	PermissionAttachment *addAttachment(Plugin *plugin, int ticks);

	void removeAttachment(PermissionAttachment *attachment);
	void recalculatePermissions();

	std::vector<PermissionAttachmentInfo *> getEffectivePermissions() const;

	void clearPermissions();

	void calculateChildPermissions(const std::map<std::string, bool> &children, bool invert, PermissionAttachment *attachment);
};