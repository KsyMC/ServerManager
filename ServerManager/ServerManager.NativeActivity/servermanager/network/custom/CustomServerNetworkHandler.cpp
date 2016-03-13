#include "CustomServerNetworkHandler.h"
#include "../../ServerManager.h"
#include "../../BanEntry.h"
#include "../../ChatColor.h"
#include "../../entity/player/SMPlayer.h"
#include "../../plugin/PluginManager.h"
#include "../../event/EventFactory.h"
#include "../../event/player/PlayerPreLoginEvent.h"
#include "../../event/player/PlayerLoginEvent.h"
#include "../../event/player/PlayerJoinEvent.h"
#include "../../event/player/PlayerCommandPreprocessEvent.h"
#include "../../event/player/PlayerChatEvent.h"
#include "../../event/player/PlayerInteractEvent.h"
#include "../../event/player/PlayerAnimationEvent.h"
#include "../../event/block/SignChangeEvent.h"
#include "../../util/SMUtil.h"
#include "minecraftpe/SharedConstants.h"
#include "minecraftpe/client/gamemode/GameMode.h"
#include "minecraftpe/world/level/block/Block.h"
#include "minecraftpe/world/level/Level.h"
#include "minecraftpe/world/level/BlockSource.h"
#include "minecraftpe/world/level/storage/LevelStorage.h"
#include "minecraftpe/world/level/Dimension.h"
#include "minecraftpe/network/RakNetInstance.h"
#include "minecraftpe/network/ServerNetworkHandler.h"
#include "minecraftpe/network/PacketSender.h"
#include "minecraftpe/network/packet/PlayStatusPacket.h"
#include "minecraftpe/network/packet/LoginPacket.h"
#include "minecraftpe/network/packet/TextPacket.h"
#include "minecraftpe/network/packet/MovePlayerPacket.h"
#include "minecraftpe/network/packet/DisconnectPacket.h"
#include "minecraftpe/network/packet/UseItemPacket.h"
#include "minecraftpe/network/packet/PlayerActionPacket.h"
#include "minecraftpe/network/packet/DropItemPacket.h"
#include "minecraftpe/network/packet/AnimatePacket.h"
#include "minecraftpe/network/packet/BlockEntityDataPacket.h"
#include "minecraftpe/platform/threading/WorkerPool.h"
#include "minecraftpe/platform/threading/BackgroundWorker.h"
#include "Substrate.h"

void(*CustomServerNetworkHandler::onDisconnect_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, const std::string &message);
void CustomServerNetworkHandler::onDisconnect(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, const std::string &message)
{
	Player *player = real->_getPlayer(guid);
	if (!player)
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (smPlayer)
		smPlayer->close(ChatColor::YELLOW + "%multiplayer.player.left", message, false);

	real->updateServerAnnouncement();
}

void(*CustomServerNetworkHandler::disconnectClient_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, const std::string &message);
void CustomServerNetworkHandler::disconnectClient(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, const std::string &message)
{
}

void(*CustomServerNetworkHandler::handleLogin_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, LoginPacket *packet);
void CustomServerNetworkHandler::handleLogin(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, LoginPacket *packet)
{
	if (!real->visible || real->_getPlayer(guid))
		return;

	if (packet->protocol1 != SharedConstants::NetworkProtocolVersion)
	{
		PlayStatusPacket statusPacket;
		if (packet->protocol1 < SharedConstants::NetworkProtocolVersion)
			statusPacket.status = Failed_ClientOld;
		else
			statusPacket.status = Failed_ServerOld;
		real->sender->send(guid, statusPacket);
		return;
	}

	std::string username = packet->username;
	const char *ipAddress = real->raknet->getPeer()->GetSystemAddressFromGuid(guid).ToString(false);

	PlayerPreLoginEvent preLoginEvent(username, ipAddress, packet->clientUUID);
	ServerManager::getPluginManager()->callEvent(preLoginEvent);

	if (preLoginEvent.getResult() != PlayerPreLoginEvent::ALLOWED)
	{
		DisconnectPacket pk(preLoginEvent.getKickMessage());
		real->sender->send(guid, pk);
		return;
	}

	bool valid = true;

	int len = username.length();
	if (len > 16 || len < 3)
		valid = false;

	for (int i = 0; i < len; i++)
	{
		char c = username[i];
		if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') || c == '_')
			continue;

		valid = false;
		break;
	}

	std::unique_ptr<ServerPlayer> player = real->createNewPlayer(guid, packet);
	std::unique_ptr<SMPlayer> smPlayer(new SMPlayer(ServerManager::getServer(), player.get()));

	PlayerLoginEvent loginEvent(smPlayer.get(), ipAddress);

	std::string iusername = SMUtil::toLower(packet->username);
	if (!valid || !iusername.compare(SMUtil::toLower(ServerManager::getLocalPlayer()->getName())) ||
		!iusername.compare("rcon") || !iusername.compare("console") || !iusername.compare("server"))
		loginEvent.disallow(PlayerLoginEvent::KICK_INVALID_NAME, "disconnectionScreen.invalidName");
	else if (packet->skin.length() != 64 * 32 * 4 && packet->skin.length() != 64 * 64 * 4)
		loginEvent.disallow(PlayerLoginEvent::KICK_INVALID_SKIN, "disconnectionScreen.invalidSkin");
	else if (ServerManager::getBanList(BanList::NAME)->isBanned(iusername))
	{
		BanEntry *entry = ServerManager::getBanList(BanList::NAME)->getBanEntry(iusername);
		loginEvent.disallow(PlayerLoginEvent::KICK_BANNED, "You are banned from this server! Reason: " + entry->getReason());
	}
	else if (ServerManager::hasWhitelist() && !ServerManager::isWhitelisted(iusername))
		loginEvent.disallow(PlayerLoginEvent::KICK_WHITELIST, "You are not white-listed on this server!");
	else if (ServerManager::getBanList(BanList::IP)->isBanned(ipAddress))
	{
		BanEntry *entry = ServerManager::getBanList(BanList::IP)->getBanEntry(ipAddress);
		loginEvent.disallow(PlayerLoginEvent::KICK_BANNED, "Your IP address is banned from this server! Reason: " + entry->getReason());
	}

	ServerManager::getPluginManager()->callEvent(loginEvent);
	if (loginEvent.getResult() != PlayerLoginEvent::ALLOWED)
	{
		smPlayer->disconnect();

		DisconnectPacket pk(loginEvent.getKickMessage());
		real->sender->send(guid, pk);
		return;
	}

	std::vector<SMPlayer *> players = ServerManager::getOnlinePlayers();
	for (size_t i = 0; i < players.size(); ++i)
	{
		SMPlayer *p = players[i];
		if (!SMUtil::toLower(p->getName()).compare(iusername))
		{
			p->kick("You logged in from another location");
			break;
		}
	}

	smPlayer->loggedIn = true;

	real->sender->send(guid, PlayStatusPacket(Success));

	Dimension *dimension = player->getDimension();
	if (!dimension)
		dimension = real->level->createDimension(DimensionId::Overworld);

	player->prepareRegion(*dimension->getChunkSource());
	real->_sendLevelData(player.get(), guid);

	ServerPlayer *serverPlayer = player.get();
	real->level->addPlayer(std::move(player));
	real->_sendAdditionalLevelData(serverPlayer, guid);
	ServerManager::getServer()->addPlayer(smPlayer.get());

	smPlayer->setAddress(real->raknet->getPeer()->GetSystemAddressFromGuid(guid).ToString(false));

	PlayerJoinEvent joinEvent(smPlayer.release(), "");
	ServerManager::getPluginManager()->callEvent(joinEvent);

	real->updateServerAnnouncement();
}

void(*CustomServerNetworkHandler::handleText_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, TextPacket *packet);
void CustomServerNetworkHandler::handleText(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, TextPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player || !player->isAlive())
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (!smPlayer || !smPlayer->isConnected())
		return;

	if (packet->type == TextPacket::TYPE_CHAT)
	{
		std::string message = packet->message;
		if (SMUtil::trim(message).empty() || message.length() >= 255)
			return;

		if (message[0] == '/')
		{
			PlayerCommandPreprocessEvent event(smPlayer, message);
			ServerManager::getPluginManager()->callEvent(event);

			if (event.isCancelled())
				return;

			message = event.getMessage();
			ServerManager::dispatchCommand(event.getPlayer(), message.erase(0, 1));
		}
		else
		{
			for (std::string msg : SMUtil::split(message, '\n'))
			{
				PlayerChatEvent event(smPlayer, msg);
				ServerManager::getPluginManager()->callEvent(event);

				if (event.isCancelled())
					break;

				ServerManager::broadcastMessage(SMUtil::format(event.getFormat().c_str(), event.getPlayer()->getDisplayName().c_str(), event.getMessage().c_str()));
			}
		}
	}
}

void(*CustomServerNetworkHandler::handleMovePlayer_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MovePlayerPacket *packet);
void CustomServerNetworkHandler::handleMovePlayer(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MovePlayerPacket *packet)
{
	Player *player = real->_getPlayer(guid, packet->uniqueID);
	if (!player || player->i1 != 0 || player->b4)
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (!smPlayer || !smPlayer->isConnected())
		return;

	Location from(smPlayer->getRegion(), smPlayer->lastPos, smPlayer->lastRotation);
	Location to = smPlayer->getLocation();

	to.setPos(packet->pos);
	to.setRotation(packet->rot);

	PlayerMoveEvent event(smPlayer, from, to);
	ServerManager::getPluginManager()->callEvent(event);

	if (event.isCancelled())
	{
		MovePlayerPacket pk;
		pk.uniqueID = smPlayer->getUniqueID();
		pk.pos = from.getPos();
		pk.rot = from.getRotation();
		pk.yaw = from.getRotation().y;
		pk.mode = MovePlayerPacket::RESET;
		pk.onGround = false;

		real->sender->send(guid, pk);
		return;
	}

	if (!to.equals(event.getTo()) && !event.isCancelled())
	{
		smPlayer->teleport(event.getTo(), PlayerTeleportEvent::UNKNOWN);
		return;
	}

	if (!from.equals(smPlayer->getLocation()) && smPlayer->justTeleported)
	{
		smPlayer->justTeleported = false;
		return;
	}

	if (!player->riding)
	{
		if (packet->pos.y < -128.0f)
			packet->pos.y = -128.0f;
		else if (packet->pos.y > 512)
			packet->pos.y = 512.0f;

		player->motion.x = 0;
		player->motion.y = 0;
		player->motion.z = 0;

		if (packet->mode != MovePlayerPacket::RESET || (packet->mode == MovePlayerPacket::RESET && !player->isAlive()))
		{
			player->_checkMovementStatistiscs(packet->pos - player->pos);
			if (player->b3 || !player->onGround)
				player->lerpTo(packet->pos, packet->rot, 3);
			else
				player->lerpTo(packet->pos, packet->rot, 1);
		}
		player->checkBlockCollisions();
		player->yaw = packet->yaw;
	}
	else
	{
		player->riding->positionRider(*player);
		player->normalTick();
		player->pos4.y = 0;
		player->moveTo(player->pos, packet->rot);
		player->yaw = packet->yaw;

		if (player->riding)
			player->riding->positionRider(*player);

		player->tick(*player->getRegion());
	}
	player->getRegion()->getDimension()->sendBroadcast(*packet, player);
}

void(*CustomServerNetworkHandler::handleRemoveBlock_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, RemoveBlockPacket *packet);
void CustomServerNetworkHandler::handleRemoveBlock(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, RemoveBlockPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player || !player->isAlive())
		return;

	handleRemoveBlock_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleEntityEvent_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, EntityEventPacket *packet);
void CustomServerNetworkHandler::handleEntityEvent(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, EntityEventPacket *packet)
{
	handleEntityEvent_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleMobEquipment_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MobEquipmentPacket *packet);
void CustomServerNetworkHandler::handleMobEquipment(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MobEquipmentPacket *packet)
{
	handleMobEquipment_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleMobArmorEquipment_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MobArmorEquipmentPacket *packet);
void CustomServerNetworkHandler::handleMobArmorEquipment(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, MobArmorEquipmentPacket *packet)
{
	handleMobArmorEquipment_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleInteract_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, InteractPacket *packet);
void CustomServerNetworkHandler::handleInteract(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, InteractPacket *packet)
{
	handleInteract_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleUseItem_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, UseItemPacket *packet);
void CustomServerNetworkHandler::handleUseItem(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, UseItemPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player || !player->isAlive())
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (!smPlayer->isConnected())
		return;

	if (packet->face == 255)
	{
		std::unique_ptr<PlayerInteractEvent> event = EventFactory::callPlayerInteractEvent(player, Action::RIGHT_CLICK_AIR, &packet->item);
		if (event && event->useItemInHand() != Event::DENY)
			real->gamemode->useItem(*player, packet->item);
	}
	else
	{
		if (player->distanceTo(Vec3(packet->pos)) >= 12)
			return;

		Block *touchedBlock = player->getRegion()->getBlock(packet->pos);
		if (touchedBlock->isType(Block::mInvisibleBedrock))
			return;

		if (!player->isSneaking())
		{
			if (touchedBlock->use(*player, packet->pos))
				return;
		}

		Vec3 oldPos = player->pos;
		player->setPos(packet->entityPos);

		real->gamemode->useItemOn(*player, &packet->item, packet->pos, packet->face,
			Vec3(packet->pos.x + packet->fPos.x, packet->pos.y + packet->fPos.y, packet->pos.z + packet->fPos.z));

		player->setPos(oldPos);
	}
}

void(*CustomServerNetworkHandler::handlePlayerAction_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, PlayerActionPacket *packet);
void CustomServerNetworkHandler::handlePlayerAction(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, PlayerActionPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player || (!player->isAlive() && packet->action != PlayerActionPacket::RESPAWN && packet->action != PlayerActionPacket::DIMENSION_CHANGE))
		return;

	handlePlayerAction_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleDropItem_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, DropItemPacket *packet);
void CustomServerNetworkHandler::handleDropItem(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, DropItemPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player)
		return;

	player->drop(&packet->item, packet->drop);
}

void(*CustomServerNetworkHandler::handleContainerClose_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerClosePacket *packet);
void CustomServerNetworkHandler::handleContainerClose(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerClosePacket *packet)
{
	handleContainerClose_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleContainerSetSlot_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerSetSlotPacket *packet);
void CustomServerNetworkHandler::handleContainerSetSlot(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerSetSlotPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player || !player->isAlive())
		return;

	handleContainerSetSlot_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleContainerSetContent_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerSetContentPacket *packet);
void CustomServerNetworkHandler::handleContainerSetContent(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, ContainerSetContentPacket *packet)
{
	handleContainerSetContent_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleCraftingEvent_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, CraftingEventPacket *packet);
void CustomServerNetworkHandler::handleCraftingEvent(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, CraftingEventPacket *packet)
{
	handleCraftingEvent_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleAnimate_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, AnimatePacket *packet);
void CustomServerNetworkHandler::handleAnimate(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, AnimatePacket *packet)
{
	Player *player = real->_getPlayer(guid, packet->uniqueID);
	if (!player || !player->isAlive())
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (!smPlayer->isConnected() || smPlayer->isLocalPlayer())
		return;

	PlayerAnimationEvent event(smPlayer, (PlayerAnimationType)packet->action);
	ServerManager::getPluginManager()->callEvent(event);

	if (event.isCancelled())
		return;

	handleAnimate_real(real, guid, packet);
}

void(*CustomServerNetworkHandler::handleBlockEntityData_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, BlockEntityDataPacket *packet);
void CustomServerNetworkHandler::handleBlockEntityData(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, BlockEntityDataPacket *packet)
{
	Player *player = real->_getPlayer(guid);
	if (!player)
		return;

	SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(player);
	if (!smPlayer->isConnected())
		return;

	BlockEntity *blockEntity = player->getRegion()->getBlockEntity(packet->pos);
	if (!blockEntity)
		return;

	if (BlockEntity::isType(*blockEntity, BlockEntityType::SIGN))
	{
		if (packet->dataTag.getString("id").compare("Sign"))
			return;

		std::vector<std::string> lines;
		lines.push_back(packet->dataTag.getString("Text1"));
		lines.push_back(packet->dataTag.getString("Text2"));
		lines.push_back(packet->dataTag.getString("Text3"));
		lines.push_back(packet->dataTag.getString("Text4"));

		SignChangeEvent event(player->getRegion()->getBlock(packet->pos), smPlayer, lines);
		ServerManager::getPluginManager()->callEvent(event);

		if (!event.isCancelled())
		{
			blockEntity->onUpdatePacket(packet->dataTag);
			player->getRegion()->getDimension()->sendBroadcast(*packet, player);
		}
	}
}

void(*CustomServerNetworkHandler::handleSpawnExperienceOrb_real)(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, SpawnExperienceOrbPacket *packet);
void CustomServerNetworkHandler::handleSpawnExperienceOrb(ServerNetworkHandler *real, const RakNet::RakNetGUID &guid, SpawnExperienceOrbPacket *packet)
{
}

void(*CustomServerNetworkHandler::allowIncomingConnections_real)(ServerNetworkHandler *real, const std::string &name, bool visible);
void CustomServerNetworkHandler::allowIncomingConnections(ServerNetworkHandler *real, const std::string &name, bool visible)
{
	real->visible = true;
	real->serverName = ServerManager::getServer()->getServerName();
	real->raknet->announceServer(real->serverName);
}

void CustomServerNetworkHandler::setupHooks()
{
	MSHookFunction((void *)&ServerNetworkHandler::onDisconnect, (void *)&onDisconnect, (void **)&onDisconnect_real);
	MSHookFunction((void *)&ServerNetworkHandler::disconnectClient, (void *)&disconnectClient, (void **)&disconnectClient_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP11LoginPacket"), (void *)&handleLogin, (void **)&handleLogin_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP10TextPacket"), (void *)&handleText, (void **)&handleText_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP16MovePlayerPacket"), (void *)&handleMovePlayer, (void **)&handleMovePlayer_real);
	/*MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP17RemoveBlockPacket"), (void *)&handleRemoveBlock, (void **)&handleRemoveBlock_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP17EntityEventPacket"), (void *)&handleEntityEvent, (void **)&handleEntityEvent_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP18MobEquipmentPacket"), (void *)&handleMobEquipment, (void **)&handleMobEquipment_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP23MobArmorEquipmentPacket"), (void *)&handleMobArmorEquipment, (void **)&handleMobArmorEquipment_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP14InteractPacket"), (void *)&handleInteract, (void **)&handleInteract_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP13UseItemPacket"), (void *)&handleUseItem, (void **)&handleUseItem_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP18PlayerActionPacket"), (void *)&handlePlayerAction, (void **)&handlePlayerAction_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP14DropItemPacket"), (void *)&handleDropItem, (void **)&handleDropItem_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP20ContainerClosePacket"), (void *)&handleContainerClose, (void **)&handleContainerClose_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP22ContainerSetSlotPacket"), (void *)&handleContainerSetSlot, (void **)&handleContainerSetSlot_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP25ContainerSetContentPacket"), (void *)&handleContainerSetContent, (void **)&handleContainerSetContent_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP19CraftingEventPacket"), (void *)&handleCraftingEvent, (void **)&handleCraftingEvent_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP13AnimatePacket"), (void *)&handleAnimate, (void **)&handleAnimate_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP21BlockEntityDataPacket"), (void *)&handleBlockEntityData, (void **)&handleBlockEntityData_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN20ServerNetworkHandler6handleERKN6RakNet10RakNetGUIDEP24SpawnExperienceOrbPacket"), (void *)&handleSpawnExperienceOrb, (void **)&handleSpawnExperienceOrb_real);*/
	MSHookFunction((void *)&ServerNetworkHandler::allowIncomingConnections, (void *)*allowIncomingConnections, (void **)&allowIncomingConnections_real);
}