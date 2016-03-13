#pragma once

#include <vector>

#include "../SMMob.h"
#include "../../command/CommandSender.h"
#include "minecraftpe/world/level/GameType.h"

class PacketSender;
class Player;
class Vec3;
class Vec2;
class Inventory;
class ItemInstance;
class Packet;
class PermissibleBase;

class SMPlayer : public SMMob, public CommandSender
{
public:
	bool loggedIn;

	Vec3 lastPos;
	Vec2 lastRotation;
	bool justTeleported;

protected:
	bool connected;

	std::string displayName;
	std::string listName;

	std::string ipAddress;

	PermissibleBase *perm;

public:
	SMPlayer(Server *server, Player *entity);
	~SMPlayer();

	virtual void sendRawMessage(const std::string &message);
	void sendMessage(const std::string &message);
	void sendMessage(const std::vector<std::string> &messages);
	void sendTranslation(const std::string &message, const std::vector<std::string> &params);
	
	virtual void sendPopup(const std::string &message, const std::string &subtitle = "");
	virtual void sendTip(const std::string &message);

	bool isLocalPlayer() const;

	virtual bool isOnline() const;
	bool isConnected() const;

	bool isOp() const;
	void setOp(bool value);

	bool isPermissionSet(const std::string &name);
	bool isPermissionSet(Permission *perm);
	bool hasPermission(const std::string &name);
	bool hasPermission(Permission *perm);

	PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value);
	PermissionAttachment *addAttachment(Plugin *plugin);
	PermissionAttachment *addAttachment(Plugin *plugin, const std::string &name, bool value, int ticks);
	PermissionAttachment *addAttachment(Plugin *plugin, int ticks);

	void removeAttachment(PermissionAttachment *attachment);
	void recalculatePermissions();

	std::vector<PermissionAttachmentInfo *> getEffectivePermissions() const;

	float getEyeHeight() const;
	float getEyeHeight(bool ignoreSneaking) const;

	void setSneaking(bool sneak);
	bool isSneaking() const;

	void setSprinting(bool sprinting);
	bool isSprinting() const;

	void dataPacket(const Packet &packet);

	const std::string &getAddress() const;
	void setAddress(const char *ipAddress);

	std::string getDisplayName() const;
	void setDisplayName(const std::string &name);

	bool teleport(const Location &location, PlayerTeleportEvent::TeleportCause cause = PlayerTeleportEvent::PLUGIN);

	void setGameType(GameType gametype);
	GameType getGameType() const;

	const std::string &getName() const;

	void kick(const std::string &reason, bool isAdmin = true);
	void close(const std::string &message, const std::string &reason, bool notify = true);

	void disconnect();

	Server *getServer() const;

	Inventory *getInventory() const;
	ItemInstance *getSelectedItem() const;

	Player *getHandle() const;
	void setHandle(Player *player);
};
