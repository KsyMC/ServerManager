#pragma once

#include <string>
#include <map>
#include <vector>
#include <json/json.h>

#include "PermissionDefault.h"

class Permissible;

class Permission
{
public:
	static const PermissionDefault DEFAULT_PERMISSION;

private:
	std::string name;
	std::map<std::string, bool> children;
	PermissionDefault defaultValue;
	std::string description;

public:
	static bool getDefaultValue(PermissionDefault def, bool op);
	static PermissionDefault getDefaultByName(const std::string &value);

	Permission(const std::string &name, const std::string &description = "", PermissionDefault defaultValue = DEFAULT_PERMISSION, const std::map<std::string, bool> &children = std::map<std::string, bool>());

	const std::string &getName() const;
	std::map<std::string, bool> &getChildren();

	PermissionDefault getDefault() const;
	void setDefault(PermissionDefault value);

	const std::string &getDescription() const;
	void setDescription(const std::string &value);

	std::vector<Permissible *> getPermissibles() const;

	void recalculatePermissibles();
	Permission *addParent(const std::string &name, bool value);
	void addParent(Permission *perm, bool value);

	static std::vector<Permission *> loadPermissions(const Json::Value &data, PermissionDefault def);
	static Permission *loadPermission(const std::string &name, const Json::Value &data);
	static Permission *loadPermission(const std::string &name, const Json::Value &data, PermissionDefault def, std::vector<Permission *> &output);
	
private:
	static std::map<std::string, bool> extractChildren(const Json::Value &input, const std::string &name, PermissionDefault def, std::vector<Permission *> &output);
};