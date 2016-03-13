#include "ServerManager.h"

Server *ServerManager::server = NULL;

Server *ServerManager::getServer()
{
	return server;
}

void ServerManager::setServer(Server *server)
{
	if (!ServerManager::server)
		ServerManager::server = server;
}

void ServerManager::deleteServer()
{
	delete server;
}

const std::vector<SMPlayer *> &ServerManager::getOnlinePlayers()
{
	return server->getOnlinePlayers();
}

SMPlayer *ServerManager::getPlayer(const std::string &name)
{
	return server->getPlayer(name);
}

std::vector<SMPlayer *> ServerManager::matchPlayer(const std::string &partialName)
{
	return server->matchPlayer(partialName);
}

SMPlayer *ServerManager::getPlayerExact(const std::string &name)
{
	return server->getPlayerExact(name);
}

SMPlayer *ServerManager::getLocalPlayer()
{
	return server->getLocalPlayer();
}

SMEntity *ServerManager::getEntity(Entity *entity)
{
	return server->getEntity(entity);
}

void ServerManager::broadcastMessage(const std::string &message)
{
	server->broadcastMessage(message);
}

void ServerManager::broadcastTranslation(const std::string &message, const std::vector<std::string> &params)
{
	server->broadcastTranslation(message, params);
}

int ServerManager::getMaxPlayers()
{
	return server->getMaxPlayers();
}

int ServerManager::getPort()
{
	return server->getPort();
}

int ServerManager::getViewDistance()
{
	return server->getViewDistance();
}

std::string ServerManager::getServerName()
{
	return server->getServerName();
}

bool ServerManager::hasWhitelist()
{
	return server->hasWhitelist();
}

void ServerManager::setWhitelist(bool value)
{
	server->setWhitelist(value);
}

bool ServerManager::dispatchCommand(SMPlayer *sender, const std::string &commandLine)
{
	return server->dispatchCommand(sender, commandLine);
}

PluginCommand *ServerManager::getPluginCommand(const std::string &name)
{
	return server->getPluginCommand(name);
}

SMLevel *ServerManager::getLevel()
{
	return server->getLevel();
}

BanList *ServerManager::getBanList(BanList::Type type)
{
	return server->getBanList(type);
}

SMList *ServerManager::getWhitelist()
{
	return server->getWhitelistList();
}

void ServerManager::banIP(const std::string &address)
{
	server->banIP(address);
}

void ServerManager::unbanIP(const std::string &address)
{
	server->unbanIP(address);
}

void ServerManager::reloadWhitelist()
{
	server->reloadWhitelist();
}

bool ServerManager::isWhitelisted(const std::string &name)
{
	return server->isWhitelisted(name);
}

CommandMap *ServerManager::getCommandMap()
{
	return server->getCommandMap();
}

PluginManager *ServerManager::getPluginManager()
{
	return server->getPluginManager();
}

