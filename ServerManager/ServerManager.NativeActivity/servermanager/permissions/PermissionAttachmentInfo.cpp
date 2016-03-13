#include "PermissionAttachmentInfo.h"

PermissionAttachmentInfo::PermissionAttachmentInfo(Permissible *permissible, const std::string &permission, PermissionAttachment *attachment, bool value)
{
	this->permissible = permissible;
	this->permission = permission;
	this->attachment = attachment;
	this->value = value;
}

Permissible *PermissionAttachmentInfo::getPermissible() const
{
	return permissible;
}

const std::string &PermissionAttachmentInfo::getPermission() const
{
	return permission;
}

PermissionAttachment *PermissionAttachmentInfo::getAttachment() const
{
	return attachment;
}

bool PermissionAttachmentInfo::getValue() const
{
	return value;
}