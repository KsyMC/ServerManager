#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

#include "../event/EventPriority.h"
#include "../event/EventType.h"

class Server;
class Command;
class CommandMap;
class Event;
class HandlerList;
class Plugin;
class Listener;
class Permission;
class Permissible;

class PluginManager
{
private:
	Server *server;
	CommandMap *commandMap;

	static std::vector<Plugin *> prePlugins;
	std::vector<Plugin *> plugins;
	std::map<std::string, Plugin *> lookupNames;

	std::map<std::string, Permission *> permissions;
	std::map<bool, std::vector<Permission *>> defaultPerms;
	std::map<std::string, std::map<Permissible *, bool>> permSubs;
	std::map<bool, std::map<Permissible *, bool>> defSubs;

	std::string pluginDir;

public:
	PluginManager(Server *instance, CommandMap *commandMap);
	~PluginManager();

	static void registerPlugin(Plugin *plugin);
	std::vector<Plugin *> loadPlugins(const std::string &pluginDir);
	Plugin *loadPlugin(Plugin *plugin);

	Plugin *getPlugin(const std::string &name) const;
	const std::vector<Plugin *> &getPlugins() const;

	bool isPluginEnabled(const std::string &name) const;
	bool isPluginEnabled(Plugin *plugin) const;

	void enablePlugin(Plugin *plugin);

	void disablePlugins();
	void disablePlugin(Plugin *plugin);

	void clearPlugins();

	void callEvent(Event &event);
	void registerEvent(EventType type, Listener *listener, std::function<void(Listener *, Event &)> func, Plugin *plugin, EventPriority priority = EventPriority::NORMAL, bool ignoreCancelled = false);

private:
	HandlerList *getEventListeners(EventType type);

public:
	Permission *getPermission(const std::string &name);
	bool addPermission(Permission *perm);
	const std::vector<Permission *> &getDefaultPermissions(bool op) const;

	void removePermission(Permission *perm);
	void removePermission(const std::string &name);

	void recalculatePermissionDefaults(Permission *perm);

private:
	void calculatePermissionDefault(Permission *perm);
	void dirtyPermissibles(bool op);

public:
	void subscribeToPermission(const std::string &permission, Permissible *permissible);
	void unsubscribeFromPermission(const std::string &permission, Permissible *permissible);

	void subscribeToDefaultPerms(bool op, Permissible *permissible);
	void unsubscribeFromDefaultPerms(bool op, Permissible *permissible);

	std::vector<Permissible *> getPermissionSubscriptions(const std::string &permission) const;
	std::vector<Permissible *> getDefaultPermSubscriptions(bool op) const;

	std::vector<Permission *> getPermissions() const;
};
