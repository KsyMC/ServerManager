#include <limits>
#include <algorithm>
#include <curl/curl.h>
#include <json/json.h>

#include "Server.h"
#include "ServerManager.h"
#include "SMList.h"
#include "BanList.h"
#include "BanEntry.h"
#include "ChatColor.h"
#include "client/settings/SMOptions.h"
#include "command/CommandMap.h"
#include "command/Command.h"
#include "level/SMLevel.h"
#include "plugin/PluginManager.h"
#include "plugin/Plugin.h"
#include "plugin/PluginDescriptionFile.h"
#include "util/SMUtil.h"
#include "util/permissions/DefaultPermissions.h"
#include "version.h"
#include "minecraftpe/world/Minecraft.h"
#include "minecraftpe/client/player/LocalPlayer.h"
#include "minecraftpe/world/entity/EntityClassTree.h"
#include "minecraftpe/util/File.h"

const std::string Server::BROADCAST_CHANNEL_ADMINISTRATIVE = "servermanager.broadcast.admin";
const std::string Server::BROADCAST_CHANNEL_USERS = "servermanager.broadcast.user";

Server::Server()
{
	ServerManager::setServer(this);

	started = false;
	level = NULL;
	server = NULL;
	localPlayer = NULL;
}

Server::~Server()
{
	delete options;
	delete banByName;
	delete banByIP;
	delete operators;
	delete whitelist;
	delete commandMap;
	delete pluginManager;
}

void Server::init(Minecraft *server, const std::string &path)
{
	this->server = server;

	serverDir = path + "servermanager/";
	pluginDir = serverDir + "plugins/";
	File::createFolder(serverDir);

	options = new SMOptions("servermanager.txt");
	options->load(serverDir);

	banByName = new BanList("banned-players.txt");
	banByName->load(serverDir);

	banByIP = new BanList("banned-ips.txt");
	banByIP->load(serverDir);

	operators = new SMList("ops.txt");
	operators->load(serverDir);

	whitelist = new SMList("white-list.txt");
	whitelist->load(serverDir);

	commandMap = new CommandMap;
	pluginManager = new PluginManager(this, commandMap);

	loadPlugins();
	enablePlugins(PluginLoadOrder::STARTUP);
}

void Server::start(LocalPlayer *localPlayer, Level *level)
{
	if (started)
		return;
	started = true;

	enablePlugins(PluginLoadOrder::POSTWORLD);

	this->level = new SMLevel(this, level);
	this->localPlayer = new SMPlayer(this, localPlayer);
	players.push_back(this->localPlayer);

	updateCheck();
}

void Server::stop()
{
	if (!started)
		return;
	started = false;

	options->save();
	banByName->save();
	banByIP->save();
	operators->save();
	whitelist->save();

	disablePlugins();

	localPlayer->disconnect();

	for (size_t i = 0; i < players.size(); ++i)
		players[i]->close(ChatColor::YELLOW + "%multiplayer.player.left", "Server Closed");

	for (size_t i = 0; i < players.size(); ++i)
		delete players[i];

	players.clear();

	pluginManager->clearPlugins();

	delete level;
	level = NULL;
}

SMOptions *Server::getOptions() const
{
	return options;
}

void Server::saveOptions()
{
	options->save();
}

void Server::setVanillaCommands()
{
}

void Server::loadPlugins()
{
	if (File::exists(pluginDir))
	{
		for (Plugin *plugin : pluginManager->loadPlugins(pluginDir))
			plugin->onLoad();
	}
	else
		File::createFolder(pluginDir);
}

void Server::loadPlugin(Plugin *plugin)
{
	pluginManager->enablePlugin(plugin);

	std::vector<Permission *> perms = plugin->getDescription()->getPermissions();
	for (Permission *perm : perms)
		pluginManager->addPermission(perm);
}

void Server::enablePlugins(PluginLoadOrder type)
{
	if (type == PluginLoadOrder::STARTUP)
	{
	}

	for (Plugin *plugin : pluginManager->getPlugins())
		if (!plugin->isEnabled() && plugin->getDescription()->getLoad() == type)
			loadPlugin(plugin);

	if (type == PluginLoadOrder::POSTWORLD)
	{
		commandMap->setFallbackCommands();
		setVanillaCommands();
		DefaultPermissions::registerCorePermissions();
	}
}

void Server::disablePlugins()
{
	pluginManager->disablePlugins();
}

const std::vector<SMPlayer *> &Server::getOnlinePlayers() const
{
	return players;
}

SMPlayer *Server::getPlayer(const std::string &name) const
{
	SMPlayer *found = NULL;
	std::string lowerName = SMUtil::toLower(name);
	int delta = std::numeric_limits<int>::max();

	for (size_t i = 0; i < players.size(); ++i)
	{
		SMPlayer *player = players[i];
		std::string n = SMUtil::toLower(player->getName());
		if (n.find(lowerName) != std::string::npos)
		{
			int curDelta = n.length() - lowerName.length();
			if (curDelta < delta)
			{
				found = player;
				delta = curDelta;
			}

			if (curDelta == 0)
				break;
		}
	}
	return found;
}

std::vector<SMPlayer *> Server::matchPlayer(const std::string &partialName) const
{
	std::string lname = SMUtil::toLower(partialName);
	std::vector<SMPlayer *> matchedPlayers;

	for (size_t i = 0; i < players.size(); ++i)
	{
		SMPlayer *iterPlayer = players[i];
		std::string iterPlayerName = iterPlayer->getName();
		if (!iterPlayerName.compare(lname))
		{
			matchedPlayers.clear();
			matchedPlayers.push_back(iterPlayer);

			break;
		}
		else if (iterPlayerName.find(lname))
			matchedPlayers.push_back(iterPlayer);
	}
	return matchedPlayers;
}

SMPlayer *Server::getPlayerExact(const std::string &name) const
{
	std::string lname = SMUtil::toLower(name);

	for (size_t i = 0; i < players.size(); ++i)
	{
		SMPlayer *player = players[i];
		if (!SMUtil::toLower(player->getName()).compare(lname))
			return player;
	}
	return NULL;
}

SMPlayer *Server::getLocalPlayer() const
{
	return localPlayer;
}

void Server::addPlayer(SMPlayer *player)
{
	if (std::find(players.begin(), players.end(), player) == players.end())
		players.push_back(player);
}

void Server::removePlayer(SMPlayer *player)
{
	auto it = std::find(players.begin(), players.end(), player);
	if (it != players.end())
		players.erase(it);
}

SMPlayer *Server::getPlayer(Player *player) const
{
	for (size_t i = 0; i < players.size(); ++i)
	{
		SMPlayer *smPlayer = players[i];
		if (smPlayer->getHandle() == player)
			return smPlayer;
	}
	return NULL;
}

void Server::removeEntity(Entity *entity)
{
	EntityUniqueID uniqueID = entity->getUniqueID();
	delete entityList[uniqueID];
	entityList.erase(uniqueID);
}

SMEntity *Server::getEntity(Entity *entity)
{
	EntityUniqueID uniqueID = entity->getUniqueID();
	if (entityList.find(uniqueID) == entityList.end())
	{
		SMEntity *smEntity = SMEntity::getEntity(this, entity);
		entityList[uniqueID] = smEntity;
		return smEntity;
	}
	return entityList[uniqueID];
}

int Server::broadcastMessage(const std::string &message)
{
	return broadcast(message, BROADCAST_CHANNEL_USERS);
}

int Server::broadcast(const std::string &message, const std::string &permission)
{
	int count = 0;
	
	std::vector<Permissible *> permissibles = getPluginManager()->getPermissionSubscriptions(permission);
	for (size_t i = 0; i < permissibles.size(); ++i)
	{
		Permissible *permissible = permissibles[i];
		if (permissible->isCommandSender() && permissible->hasPermission(permission))
		{
			CommandSender *user = (CommandSender *)permissible;
			user->sendMessage(message);
			count++;
		}
	}
	return count;
}

int Server::broadcastTranslation(const std::string &message, const std::vector<std::string> &params)
{
	return broadcast(message, params, BROADCAST_CHANNEL_USERS);
}

int Server::broadcast(const std::string &message, const std::vector<std::string> &params, const std::string &permission)
{
	int count = 0;

	std::vector<Permissible *> permissibles = getPluginManager()->getPermissionSubscriptions(permission);
	for (size_t i = 0; i < permissibles.size(); ++i)
	{
		Permissible *permissible = permissibles[i];
		if (permissible->isCommandSender() && permissible->hasPermission(permission))
		{
			CommandSender *user = (CommandSender *)permissible;
			user->sendTranslation(message, params);
			count++;
		}
	}
	return count;
}

int Server::getMaxPlayers() const
{
	return options->getServerPlayers();
}

int Server::getPort() const
{
	return options->getServerPort();
}

int Server::getViewDistance() const
{
	return options->getViewDistance();
}

std::string Server::getServerName() const
{
	return options->getServerName();
}

bool Server::hasWhitelist() const
{
	return options->hasWhitelist();
}

void Server::setWhitelist(bool value)
{
	options->setWhitelist(value);
}

bool Server::getPvP() const
{
	return options->getPvP();
}

void Server::setPvP(bool value)
{
	options->setPvP(value);
}

void Server::reload()
{
	options->load(serverDir);
	banByName->load(serverDir);
	banByIP->load(serverDir);
	operators->load(serverDir);
	whitelist->load(serverDir);

	pluginManager->clearPlugins();
	commandMap->clearCommands();

	loadPlugins();
	enablePlugins(PluginLoadOrder::STARTUP);
	enablePlugins(PluginLoadOrder::POSTWORLD);
}

bool Server::dispatchCommand(CommandSender *sender, const std::string &commandLine)
{
	if (commandMap->dispatch(sender, commandLine))
		return true;

	sender->sendTranslation(ChatColor::RED + "%commands.generic.notFound", {});
	return false;
}

PluginCommand *Server::getPluginCommand(const std::string &name)
{
	Command *command = commandMap->getCommand(name);
	if (command->isPluginCommand())
		return (PluginCommand *)command;
	return NULL;
}

SMLevel *Server::getLevel() const
{
	return level;
}

BanList *Server::getBanList(BanList::Type type) const
{
	switch (type)
	{
	case BanList::IP:
		return banByIP;
	case BanList::NAME:
	default:
		return banByName;
	}
}

SMList *Server::getWhitelistList() const
{
	return whitelist;
}

SMList *Server::getOPList() const
{
	return operators;
}

void Server::banIP(const std::string &address)
{
	BanEntry *entry = new BanEntry(address);

	banByIP->add(entry);
	banByIP->save();
}

void Server::unbanIP(const std::string &address)
{
	banByIP->pardon(address);
	banByIP->save();
}

void Server::addWhitelist(const std::string &name)
{
	whitelist->add(name);
	whitelist->save();
}

void Server::removeWhitelist(const std::string &name)
{
	whitelist->remove(name);
	whitelist->save();
}

void Server::reloadWhitelist()
{
	whitelist->reload();
}

void Server::addOp(const std::string &name)
{
	operators->add(SMUtil::toLower(name));
	operators->save();
}

void Server::removeOp(const std::string &name)
{
	operators->remove(SMUtil::toLower(name));
	operators->save();
}

bool Server::isWhitelisted(const std::string &name) const
{
	return !options->hasWhitelist() || operators->isExist(SMUtil::toLower(name)) || whitelist->isExist(SMUtil::toLower(name));
}

bool Server::isOp(const std::string &name) const
{
	return operators->isExist(SMUtil::toLower(name));
}

CommandMap *Server::getCommandMap() const
{
	return commandMap;
}

PluginManager *Server::getPluginManager() const
{
	return pluginManager;
}

std::string Server::getGamemodeString(GameType type)
{
	switch (type)
	{
	case GameType::SURVIVAL:
		return "%gameMode.survival";
	case GameType::CREATIVE:
		return "%gameMode.creative";
	case GameType::ADVENTURE:
		return "%gameMode.adventure";
	case GameType::SPECTATOR:
		return "%gameMode.spectator";
	}
	return "UNKNOWN";
}

GameType Server::getGamemodeFromString(const std::string &value)
{
	std::string newStr = SMUtil::toLower(SMUtil::trim(value.c_str()));

	if (!newStr.compare("0") || !newStr.compare("survival") || !newStr.compare("s"))
		return GameType::SURVIVAL;
	else if (!newStr.compare("1") || !newStr.compare("creative") || !newStr.compare("c"))
		return GameType::CREATIVE;
	else if (!newStr.compare("2") || !newStr.compare("adventure") || !newStr.compare("a"))
		return GameType::ADVENTURE;
	else if (!newStr.compare("3") || !newStr.compare("spectator") || !newStr.compare("view") || !newStr.compare("v"))
		return GameType::SPECTATOR;

	return (GameType)-1;
}

Minecraft *Server::getServer() const
{
	return server;
}

bool Server::updateCheck()
{
	std::string data;
	CURL *curl = curl_easy_init();
	if (!curl)
		return false;

	struct Writer
	{
		static size_t write(char *data, size_t size, size_t nmemb, std::string *writerData)
		{
			writerData->append(data, size * nmemb);
			return size * nmemb;
		}
	};

	curl_easy_setopt(curl, CURLOPT_URL, "https://rawgit.com/KsyMC/9e8670ccfaa9c8b1dfd4/raw/servermanager.json");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer::write);

	CURLcode rc = curl_easy_perform(curl);
	if (CURLE_OK != rc)
		return false;

	curl_easy_cleanup(curl);

	if (!data.empty())
	{
		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(data, root))
			return false;

		std::string version = root.get("version", "").asString("");
		int versionCode = root.get("version-code", 0).asInt(0);

		std::vector<std::string> changelog;
		for (Json::Value log : root["changelog"])
			changelog.push_back(log.asString(""));

		if (versionCode != VERSION_CODE)
		{
			if (versionCode > VERSION_CODE)
				localPlayer->sendMessage("[SM] 새로운 버전이 있습니다 : v" + version);
			else
				localPlayer->sendMessage("[SM] 현재 개발 버전을 사용중입니다.");
		}
	}
	return true;
}