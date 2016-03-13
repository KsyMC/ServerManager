#include "SMPlayer.h"
#include "../../Server.h"
#include "../../BanList.h"
#include "../../BanEntry.h"
#include "../../ChatColor.h"
#include "../../level/SMLevel.h"
#include "../../level/SMBlockSource.h"
#include "../../event/player/PlayerGameModeChangeEvent.h"
#include "../../event/player/PlayerKickEvent.h"
#include "../../event/player/PlayerQuitEvent.h"
#include "../../plugin/PluginManager.h"
#include "../../permissions/PermissibleBase.h"
#include "../../network/PacketID.h"
#include "../../util/SMUtil.h"
#include "minecraftpe/locale/I18n.h"
#include "minecraftpe/world/entity/player/Player.h"
#include "minecraftpe/server/ServerPlayer.h"
#include "minecraftpe/network/PacketSender.h"
#include "minecraftpe/network/packet/TextPacket.h"
#include "minecraftpe/network/packet/MovePlayerPacket.h"
#include "minecraftpe/network/packet/DisconnectPacket.h"
#include "minecraftpe/world/level/Level.h"
#include "minecraftpe/world/level/BlockSource.h"
#include "minecraftpe/world/level/storage/LevelStorage.h"
#include "minecraftpe/client/game/MinecraftClient.h"
#include "minecraftpe/client/gui/gui/Gui.h"
#include "minecraftpe/client/player/LocalPlayer.h"

SMPlayer::SMPlayer(Server *server, Player *entity)
	: SMMob(server, entity)
{
	connected = true;
	justTeleported = false;

	displayName = entity->username;
	listName = entity->username;

	if (entity->isLocalPlayer())
	{
		loggedIn = true;
		setAddress("127.0.0.1");
	}

	perm = new PermissibleBase(this);
}

SMPlayer::~SMPlayer()
{
	delete perm;
}

void SMPlayer::sendRawMessage(const std::string &message)
{
	if (isLocalPlayer())
	{
		LocalPlayer *player = (LocalPlayer *)getHandle();
		Gui *gui = player->client->getGui();
		for (std::string m : SMUtil::split(message, '\n'))
			if (!m.empty())
				gui->displayClientMessage(m);
	}
	else
	{
		for (std::string m : SMUtil::split(message, '\n'))
		{
			if (m.empty())
				continue;

			dataPacket(TextPacket(TextPacket::TYPE_RAW, "", m, {}));
		}
	}
}

void SMPlayer::sendMessage(const std::string &message)
{
	sendRawMessage(message);
}

void SMPlayer::sendMessage(const std::vector<std::string> &messages)
{
	for (std::string message : messages)
		sendMessage(message);
}

void SMPlayer::sendTranslation(const std::string &message, const std::vector<std::string> &params)
{
	if (isLocalPlayer())
	{
		LocalPlayer *player = (LocalPlayer *)getHandle();
		player->client->getGui()->displayLocalizableMessage(message, params);
	}
	else
		dataPacket(TextPacket(TextPacket::TYPE_TRANSLATION, "", message, params));
}

void SMPlayer::sendPopup(const std::string &message, const std::string &subtitle)
{
	if (isLocalPlayer())
	{
		LocalPlayer *player = (LocalPlayer *)getHandle();
		player->client->getGui()->showPopupNotice(message, subtitle);
	}
	else
		dataPacket(TextPacket(TextPacket::TYPE_POPUP, message, subtitle, {}));
}

void SMPlayer::sendTip(const std::string &message)
{
	if (isLocalPlayer())
	{
		LocalPlayer *player = (LocalPlayer *)getHandle();
		player->client->getGui()->showTipMessage(message);
	}
	else
		dataPacket(TextPacket(TextPacket::TYPE_TIP, "", message, {}));
}

bool SMPlayer::isLocalPlayer() const
{
	return getHandle()->isLocalPlayer();
}

bool SMPlayer::isOnline() const
{
	return connected && loggedIn;
}

bool SMPlayer::isConnected() const
{
	return connected;
}

bool SMPlayer::isOp() const
{
	if (isLocalPlayer())
		return true;
	return server->isOp(getName());
}

void SMPlayer::setOp(bool value)
{
	if (value == isOp() || isLocalPlayer())
		return;

	if (value)
		server->addOp(getName());
	else
		server->removeOp(getName());

	perm->recalculatePermissions();
}

bool SMPlayer::isPermissionSet(const std::string &name)
{
	return perm->isPermissionSet(name);
}

bool SMPlayer::isPermissionSet(Permission *perm)
{
	return this->perm->isPermissionSet(perm);
}

bool SMPlayer::hasPermission(const std::string &name)
{
	return perm->hasPermission(name);
}

bool SMPlayer::hasPermission(Permission *perm)
{
	return this->perm->hasPermission(perm);
}

PermissionAttachment *SMPlayer::addAttachment(Plugin *plugin, const std::string &name, bool value)
{
	return perm->addAttachment(plugin, name, value);
}

PermissionAttachment *SMPlayer::addAttachment(Plugin *plugin)
{
	return perm->addAttachment(plugin);
}

PermissionAttachment *SMPlayer::addAttachment(Plugin *plugin, const std::string &name, bool value, int ticks)
{
	return perm->addAttachment(plugin, name, value, ticks);
}

PermissionAttachment *SMPlayer::addAttachment(Plugin *plugin, int ticks)
{
	return perm->addAttachment(plugin, ticks);
}

void SMPlayer::removeAttachment(PermissionAttachment *attachment)
{
	perm->removeAttachment(attachment);
}

void SMPlayer::recalculatePermissions()
{
	perm->recalculatePermissions();
}

std::vector<PermissionAttachmentInfo *> SMPlayer::getEffectivePermissions() const
{
	return perm->getEffectivePermissions();
}

float SMPlayer::getEyeHeight() const
{
	return getEyeHeight(false);
}

float SMPlayer::getEyeHeight(bool ignoreSneaking) const
{
	if (ignoreSneaking)
		return 1.62f;
	else
	{
		if (isSneaking())
			return 1.54f;
		else
			return 1.62;
	}
}

void SMPlayer::setSneaking(bool sneak)
{
	getHandle()->setSneaking(sneak);
}

bool SMPlayer::isSneaking() const
{
	return getHandle()->isSneaking();
}

void SMPlayer::setSprinting(bool sprinting)
{
	getHandle()->setSprinting(sprinting);
}

bool SMPlayer::isSprinting() const
{
	return getHandle()->isSprinting();
}

void SMPlayer::dataPacket(const Packet &packet)
{
	if (!connected)
		return;

	getHandle()->packetSender->send(getHandle()->guid, packet);
}

const std::string &SMPlayer::getAddress() const
{
	return ipAddress;
}

void SMPlayer::setAddress(const char *ipAddress)
{
	this->ipAddress = ipAddress;
}

std::string SMPlayer::getDisplayName() const
{
	return displayName;
}

void SMPlayer::setDisplayName(const std::string &name)
{
	displayName = name;
}

bool SMPlayer::teleport(const Location &location, PlayerTeleportEvent::TeleportCause cause)
{
	if (getHealth() <= 0 || isDead())
		return false;

	Location from = getLocation();
	Location to = location;

	PlayerTeleportEvent event(this, from, to, cause);
	server->getPluginManager()->callEvent(event);

	if (event.isCancelled())
		return false;

	if (getHandle()->isRiding())
		getHandle()->stopRiding(false);

	from = event.getFrom();
	to = event.getTo();

	DimensionId fromId = from.getRegion()->getHandle()->getDimensionId();
	DimensionId toId = to.getRegion()->getHandle()->getDimensionId();

	if (getHandle()->activeContainer)
		getHandle()->deleteContainerManager();

	if (fromId == toId)
		getHandle()->moveTo(to.getPos(), to.getRotation());
	else
	{
		getHandle()->changeDimension(toId);
		getHandle()->moveTo(to.getPos(), to.getRotation());
	}

	if (!isLocalPlayer())
	{
		MovePlayerPacket pk;
		pk.uniqueID = getHandle()->getUniqueID();
		pk.pos = to.getPos();
		pk.rot = to.getRotation();
		pk.yaw = to.getRotation().y;
		pk.mode = MovePlayerPacket::RESET;
		pk.onGround = false;
		dataPacket(pk);
	}

	lastPos = to.getPos();
	lastRotation = to.getRotation();
	justTeleported = true;

	return true;
}

void SMPlayer::setGameType(GameType gametype)
{
	if (gametype != getGameType())
	{
		PlayerGameModeChangeEvent event(this, gametype);
		server->getPluginManager()->callEvent(event);

		if (event.isCancelled())
			return;

		getHandle()->setPlayerGameType(gametype);
		getHandle()->fallDistance = 0;
	}
}

GameType SMPlayer::getGameType() const
{
	return getHandle()->gameType;
}

const std::string &SMPlayer::getName() const
{
	return getHandle()->username;
}

void SMPlayer::kick(const std::string &reason, bool isAdmin)
{
	if (isLocalPlayer() || !loggedIn)
		return;

	PlayerKickEvent event(this, reason, ChatColor::YELLOW + "%multiplayer.player.left");
	server->getPluginManager()->callEvent(event);

	if (event.isCancelled())
		return;

	close(event.getLeaveMessage(), event.getReason());
}

void SMPlayer::close(const std::string &message, const std::string &reason, bool notify)
{
	if (isLocalPlayer())
		return;

	if (connected)
	{
		if (notify && !reason.empty())
			dataPacket(DisconnectPacket(reason));

		connected = false;

		region->getHandle()->getLevel()->getLevelStorage()->save(*getHandle());

		PlayerQuitEvent quitEvent(this, message, { getName() });
		server->getPluginManager()->callEvent(quitEvent);

		loggedIn = false;

		server->broadcastTranslation(quitEvent.getQuitMessage(), quitEvent.getQuitParams());

		ServerPlayer *serverPlayer = (ServerPlayer *)getHandle();
		serverPlayer->disconnect();
		serverPlayer->remove();
	}
	disconnect();

	server->removePlayer(this);
}

void SMPlayer::disconnect()
{
	perm->clearPermissions();
}

Server *SMPlayer::getServer() const
{
	return server;
}

Inventory *SMPlayer::getInventory() const
{
	return getHandle()->inventory;
}

ItemInstance *SMPlayer::getSelectedItem() const
{
	return getHandle()->getSelectedItem();
}

Player *SMPlayer::getHandle() const
{
	return (Player *)entity;
}

void SMPlayer::setHandle(Player *player)
{
	entity = player;
}