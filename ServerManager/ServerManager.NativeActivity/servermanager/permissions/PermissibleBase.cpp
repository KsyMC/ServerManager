#include "PermissibleBase.h"

#include "../ServerManager.h"
#include "Permission.h"
#include "PermissionAttachment.h"
#include "PermissionAttachmentInfo.h"
#include "PermissionRemovedExecutor.h"
#include "../plugin/PluginManager.h"
#include "../plugin/Plugin.h"
#include "../util/SMUtil.h"

#include <algorithm>

PermissibleBase::PermissibleBase(ServerOperator *opable)
{
	this->opable = opable;
	parent = this;

	if (opable->isPermissible())
		this->parent = (Permissible *)opable;

	recalculatePermissions();
}

bool PermissibleBase::isOp() const
{
	return opable && opable->isOp();
}

void PermissibleBase::setOp(bool value)
{
	if (opable)
		opable->setOp(value);
}

bool PermissibleBase::isPermissionSet(const std::string &name)
{
	return permissions.find(SMUtil::toLower(name)) != permissions.end();
}

bool PermissibleBase::isPermissionSet(Permission *perm)
{
	return isPermissionSet(perm->getName());
}

bool PermissibleBase::hasPermission(const std::string &inName)
{
	std::string name = SMUtil::toLower(inName);
	if (isPermissionSet(name))
		return permissions[name]->getValue();

	Permission *perm = ServerManager::getPluginManager()->getPermission(name);
	if (perm)
		return Permission::getDefaultValue(perm->getDefault(), isOp());

	return Permission::getDefaultValue(Permission::DEFAULT_PERMISSION, isOp());
}

bool PermissibleBase::hasPermission(Permission *perm)
{
	std::string name = SMUtil::toLower(perm->getName());
	if (isPermissionSet(name))
		return permissions[name]->getValue();

	return Permission::getDefaultValue(perm->getDefault(), isOp());
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin *plugin, const std::string &name, bool value)
{
	if (!plugin || !plugin->isEnabled())
		return NULL;

	PermissionAttachment *result = new PermissionAttachment(plugin, parent);

	attachments.push_back(result);
	recalculatePermissions();

	return result;
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin *plugin)
{
	if (!plugin || !plugin->isEnabled())
		return NULL;

	PermissionAttachment *result = new PermissionAttachment(plugin, parent);

	attachments.push_back(result);
	recalculatePermissions();

	return result;
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin *plugin, const std::string &name, bool value, int ticks)
{
	if (!plugin || !plugin->isEnabled())
		return NULL;

	PermissionAttachment *result = addAttachment(plugin, ticks);

	if (result)
		result->setPermission(name, value);

	return result;
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin *plugin, int ticks)
{
	if (!plugin || !plugin->isEnabled())
		return NULL;

	PermissionAttachment *result = addAttachment(plugin);

	return result;
}

void PermissibleBase::removeAttachment(PermissionAttachment *attachment)
{
	if (!attachment)
		return;

	auto it = std::find(attachments.begin(), attachments.end(), attachment);
	if (it != attachments.end())
	{
		attachments.erase(it);
		PermissionRemovedExecutor *ex = attachment->getRemovalCallback();

		if (ex)
			ex->attachmentRemoved(attachment);

		recalculatePermissions();
	}
}

void PermissibleBase::recalculatePermissions()
{
	clearPermissions();
	std::vector<Permission *> defaults = ServerManager::getPluginManager()->getDefaultPermissions(isOp());
	ServerManager::getPluginManager()->subscribeToDefaultPerms(isOp(), parent);

	for (Permission *perm : defaults)
	{
		std::string name = SMUtil::toLower(perm->getName());
		permissions[name] = new PermissionAttachmentInfo(parent, name, NULL, true);
		ServerManager::getPluginManager()->subscribeToPermission(name, parent);
		calculateChildPermissions(perm->getChildren(), false, NULL);
	}

	for (size_t i = 0; i < attachments.size(); ++i)
		calculateChildPermissions(attachments[i]->getPermissions(), false, attachments[i]);
}

std::vector<PermissionAttachmentInfo *> PermissibleBase::getEffectivePermissions() const
{
	std::vector<PermissionAttachmentInfo *> values;
	for (auto it = permissions.begin(); it != permissions.end(); ++it)
		values.push_back(it->second);

	return values;
}

void PermissibleBase::clearPermissions()
{
	for (auto it = permissions.begin(); it != permissions.end(); ++it)
		ServerManager::getPluginManager()->unsubscribeFromPermission(it->first, parent);

	ServerManager::getPluginManager()->unsubscribeFromDefaultPerms(false, parent);
	ServerManager::getPluginManager()->unsubscribeFromDefaultPerms(true, parent);

	permissions.clear();
}

void PermissibleBase::calculateChildPermissions(const std::map<std::string, bool> &children, bool invert, PermissionAttachment *attachment)
{
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		Permission *perm = ServerManager::getPluginManager()->getPermission(it->first);
		bool value = it->second ^ invert;
		std::string lname = SMUtil::toLower(it->first);

		permissions[lname] = new PermissionAttachmentInfo(parent, lname, attachment, value);
		ServerManager::getPluginManager()->subscribeToPermission(it->first, parent);

		if (perm)
			calculateChildPermissions(perm->getChildren(), !value, attachment);
	}
}