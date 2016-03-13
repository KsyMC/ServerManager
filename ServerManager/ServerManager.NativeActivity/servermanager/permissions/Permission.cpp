#include "Permission.h"
#include "../ServerManager.h"
#include "Permissible.h"
#include "../plugin/PluginManager.h"
#include "../util/SMUtil.h"

const PermissionDefault Permission::DEFAULT_PERMISSION = PermissionDefault::OP;

bool Permission::getDefaultValue(PermissionDefault def, bool op)
{
	switch (def)
	{
	case PermissionDefault::TRUE:
		return true;
	case PermissionDefault::FALSE:
		return false;
	case PermissionDefault::OP:
		return op;
	case PermissionDefault::NOT_OP:
		return !op;
	default:
		return false;
	}
}

PermissionDefault Permission::getDefaultByName(const std::string &value)
{
	std::string name = SMUtil::toString(value);
	if (!name.compare("op") || !name.compare("isop") || !name.compare("operator") || !name.compare("isoperator") || !name.compare("admin") || !name.compare("isadmin"))
		return PermissionDefault::OP;
	else if (!name.compare("!op") || !name.compare("notop") || !name.compare("!operator") || !name.compare("notoperator") || !name.compare("!admin") || !name.compare("notadmin"))
		return PermissionDefault::NOT_OP;
	else if (!name.compare("true"))
		return PermissionDefault::TRUE;
	else if (!name.compare("false"))
		return PermissionDefault::FALSE;

	return PermissionDefault::FALSE;
}

Permission::Permission(const std::string &name, const std::string &description, PermissionDefault defaultValue, const std::map<std::string, bool> &children)
{
	this->name = name;
	this->description = description;
	this->defaultValue = defaultValue;
	this->children = children;

	recalculatePermissibles();
}

const std::string &Permission::getName() const
{
	return name;
}

std::map<std::string, bool> &Permission::getChildren()
{
	return children;
}

PermissionDefault Permission::getDefault() const
{
	return defaultValue;
}

void Permission::setDefault(PermissionDefault value)
{
	defaultValue = value;
	recalculatePermissibles();
}

const std::string &Permission::getDescription() const
{
	return description;
}

void Permission::setDescription(const std::string &value)
{
	description = value;
}

std::vector<Permissible *> Permission::getPermissibles() const
{
	return ServerManager::getPluginManager()->getPermissionSubscriptions(name);
}

void Permission::recalculatePermissibles()
{
	ServerManager::getPluginManager()->recalculatePermissionDefaults(this);

	for (Permissible *p : getPermissibles())
		p->recalculatePermissions();
}

Permission *Permission::addParent(const std::string &name, bool value)
{
	PluginManager *pm = ServerManager::getPluginManager();
	std::string lname = SMUtil::toLower(name);

	Permission *perm = pm->getPermission(lname);

	if (!perm)
	{
		perm = new Permission(lname);
		pm->addPermission(perm);
	}
	addParent(perm, value);

	return perm;
}

void Permission::addParent(Permission *perm, bool value)
{
	perm->getChildren()[getName()] = value;
	perm->recalculatePermissibles();
}

std::vector<Permission *> Permission::loadPermissions(const Json::Value &data, PermissionDefault def)
{
	std::vector<Permission *> result;

	int permIndex = 0;
	Json::Value::Members permNames = data.getMemberNames();
	for (Json::Value perm : data)
	{
		result.push_back(Permission::loadPermission(permNames[permIndex], perm, def, result));
		++permIndex;
	}

	return result;
}

Permission *Permission::loadPermission(const std::string &name, const Json::Value &data)
{
	std::vector<Permission *> output;
	return Permission::loadPermission(name, data, DEFAULT_PERMISSION, output);
}

Permission *Permission::loadPermission(const std::string &name, const Json::Value &data, PermissionDefault def, std::vector<Permission *> &output)
{
	std::string desc;
	std::map<std::string, bool> children;

	if (data.isMember("default"))
		def = Permission::getDefaultByName(data.get("default", "").asString(""));

	if (data.isMember("children"))
		children = extractChildren(data["children"], name, def, output);

	if (data.isMember("description"))
		desc = data.get("description", "").asString("");

	return new Permission(name, desc, def, children);
}

std::map<std::string, bool> Permission::extractChildren(const Json::Value &input, const std::string &name, PermissionDefault def, std::vector<Permission *> &output)
{
	std::map<std::string, bool> children;

	int permIndex = 0;
	Json::Value::Members permNames = input.getMemberNames();
	for (Json::Value perm : input)
	{
		if (perm.isBool())
			children[permNames[permIndex]] = perm.asBool(false);
		else if (perm.isObject())
		{
			Permission *perm = Permission::loadPermission(permNames[permIndex], perm, def, output);
			children[perm->getName()] = true;
			output.push_back(perm);
		}
		++permIndex;
	}

	return children;
}