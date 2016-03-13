#pragma once

#include <string>

class Permissible;
class PermissionAttachment;

class PermissionAttachmentInfo
{
private:
	Permissible *permissible;
	std::string permission;
	PermissionAttachment *attachment;
	bool value;

public:
	PermissionAttachmentInfo(Permissible *permissible, const std::string &permission, PermissionAttachment *attachment, bool value);

	Permissible *getPermissible() const;
	const std::string &getPermission() const;
	PermissionAttachment *getAttachment() const;
	bool getValue() const;
};