#pragma once

#include "Server.h"

class ServerManager
{
private:
	static Server *server;

public:
	static Server *getServer();
	static void setServer(Server *server);
	static void deleteServer();

	static const std::vector<SMPlayer *> &getOnlinePlayers();
	static SMPlayer *getPlayer(const std::string &name);
	static std::vector<SMPlayer *> matchPlayer(const std::string &partialName);
	static SMPlayer *getPlayerExact(const std::string &name);
	static SMPlayer *getLocalPlayer();

	static SMEntity *getEntity(Entity *entity);

	static void broadcastMessage(const std::string &message);
	static void broadcastTranslation(const std::string &message, const std::vector<std::string> &params);
	
	static int getMaxPlayers();
	static int getPort();
	static int getViewDistance();
	static std::string getServerName();
	static bool hasWhitelist();
	static void setWhitelist(bool value);

	static bool dispatchCommand(SMPlayer *sender, const std::string &commandLine);

	static PluginCommand *getPluginCommand(const std::string &name);

	static SMLevel *getLevel();

	static BanList *getBanList(BanList::Type type);
	static SMList *getWhitelist();

	static void banIP(const std::string &address);
	static void unbanIP(const std::string &address);

	static void reloadWhitelist();

	static bool isWhitelisted(const std::string &name);

	static CommandMap *getCommandMap();
	static PluginManager *getPluginManager();
};
