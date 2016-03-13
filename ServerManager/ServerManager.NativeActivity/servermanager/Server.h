#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "BanList.h"
#include "entity/player/SMPlayer.h"
#include "plugin/PluginLoadOrder.h"
#include "minecraftpe/world/level/GameType.h"
#include "minecraftpe/world/entity/EntityUniqueID.h"

class SMLevel;
class SMOptions;
class BanList;
class SMList;
class SMPlayer;
class CommandMap;
class Level;
class PluginManager;
class Minecraft;
class LocalPlayer;
class SMEntity;
class Plugin;
class PluginCommand;
class Player;
class Entity;

class Server
{
public:
	static const std::string BROADCAST_CHANNEL_ADMINISTRATIVE;
	static const std::string BROADCAST_CHANNEL_USERS;

private:
	bool started;

	std::string serverDir;
	std::string pluginDir;

	SMLevel *level;
	Minecraft *server;
	SMOptions *options;

	BanList *banByName;
	BanList *banByIP;
	SMList *operators;
	SMList *whitelist;

	CommandMap *commandMap;
	PluginManager *pluginManager;

	SMPlayer *localPlayer;

	std::map<EntityUniqueID, SMEntity *> entityList;
	std::vector<SMPlayer *> players;

public:
	Server();
	~Server();

	void init(Minecraft *server, const std::string &path);

	void start(LocalPlayer *localPlayer, Level *level);
	void stop();

	SMOptions *getOptions() const;
	void saveOptions();

	void loadPlugins();
	
private:
	void setVanillaCommands();

	void loadPlugin(Plugin *plugin);

public:
	void enablePlugins(PluginLoadOrder type);
	void disablePlugins();

	const std::vector<SMPlayer *> &getOnlinePlayers() const;
	SMPlayer *getPlayer(const std::string &name) const;
	std::vector<SMPlayer *> matchPlayer(const std::string &partialName) const;
	SMPlayer *getPlayerExact(const std::string &name) const;
	SMPlayer *getLocalPlayer() const;

	void addPlayer(SMPlayer *player);
	void removePlayer(SMPlayer *player);
	SMPlayer *getPlayer(Player *player) const;

	void removeEntity(Entity *entity);
	SMEntity *getEntity(Entity *entity);

	int broadcastMessage(const std::string &message);
	int broadcast(const std::string &message, const std::string &permission);

	int broadcastTranslation(const std::string &message, const std::vector<std::string> &params);
	int broadcast(const std::string &message, const std::vector<std::string> &params, const std::string &permission);

	int getMaxPlayers() const;
	int getPort() const;
	int getViewDistance() const;
	std::string getServerName() const;
	bool hasWhitelist() const;
	void setWhitelist(bool value);
	bool getPvP() const;
	void setPvP(bool value);

	void reload();

	bool dispatchCommand(CommandSender *sender, const std::string &commandLine);

	PluginCommand *getPluginCommand(const std::string &name);

	SMLevel *getLevel() const;

	BanList *getBanList(BanList::Type type) const;
	SMList *getWhitelistList() const;
	SMList *getOPList() const;

	void banIP(const std::string &address);
	void unbanIP(const std::string &address);

	void addWhitelist(const std::string &name);
	void removeWhitelist(const std::string &name);
	void reloadWhitelist();

	void addOp(const std::string &name);
	void removeOp(const std::string &name);

	bool isWhitelisted(const std::string &name) const;
	bool isOp(const std::string &name) const;

	CommandMap *getCommandMap() const;
	PluginManager *getPluginManager() const;

	static std::string getGamemodeString(GameType type);
	static GameType getGamemodeFromString(const std::string &value);

	Minecraft *getServer() const;

private:
	bool updateCheck();
};
