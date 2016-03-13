#pragma once

#include <string>
#include <raknet/RakNetTypes.h>

#include "minecraftpe/server/ServerPlayer.h"

class ServerNetworkHandler;
class LoginPacket;
class SetTimePacket;
class TextPacket;
class MoveEntityPacket;
class MovePlayerPacket;
class RemoveBlockPacket;
class EntityEventPacket;
class MobEquipmentPacket;
class MobArmorEquipmentPacket;
class InteractPacket;
class UseItemPacket;
class PlayerActionPacket;
class DropItemPacket;
class ContainerClosePacket;
class ContainerSetSlotPacket;
class ContainerSetContentPacket;
class CraftingEventPacket;
class AnimatePacket;
class BlockEntityDataPacket;
class PlayerInputPacket;
class SpawnExperienceOrbPacket;
class SMPlayer;

class CustomServerNetworkHandler
{
public:
	static void(*onDisconnect_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, const std::string &);
	static void onDisconnect(ServerNetworkHandler *, const RakNet::RakNetGUID &, const std::string &);

	static void(*disconnectClient_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, const std::string &);
	static void disconnectClient(ServerNetworkHandler *, const RakNet::RakNetGUID &, const std::string &);

	static void(*handleLogin_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, LoginPacket *);
	static void handleLogin(ServerNetworkHandler *, const RakNet::RakNetGUID &, LoginPacket *);

	static void (*handleSetTime_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, SetTimePacket *);
	static void handleSetTime(ServerNetworkHandler *, const RakNet::RakNetGUID &, SetTimePacket *);

	static void(*handleText_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, TextPacket *);
	static void handleText(ServerNetworkHandler *, const RakNet::RakNetGUID &, TextPacket *);

	static void(*handleMoveEntity_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, MoveEntityPacket *);
	static void handleMoveEntity(ServerNetworkHandler *, const RakNet::RakNetGUID &, MoveEntityPacket *);

	static void(*handleMovePlayer_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, MovePlayerPacket *);
	static void handleMovePlayer(ServerNetworkHandler *, const RakNet::RakNetGUID &, MovePlayerPacket *);

	static void(*handleRemoveBlock_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, RemoveBlockPacket *);
	static void handleRemoveBlock(ServerNetworkHandler *, const RakNet::RakNetGUID &, RemoveBlockPacket *);

	static void(*handleEntityEvent_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, EntityEventPacket *);
	static void handleEntityEvent(ServerNetworkHandler *, const RakNet::RakNetGUID &, EntityEventPacket *);

	static void(*handleMobEquipment_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, MobEquipmentPacket *);
	static void handleMobEquipment(ServerNetworkHandler *, const RakNet::RakNetGUID &, MobEquipmentPacket *);

	static void(*handleMobArmorEquipment_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, MobArmorEquipmentPacket *);
	static void handleMobArmorEquipment(ServerNetworkHandler *, const RakNet::RakNetGUID &, MobArmorEquipmentPacket *);

	static void(*handleInteract_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, InteractPacket *);
	static void handleInteract(ServerNetworkHandler *, const RakNet::RakNetGUID &, InteractPacket *);

	static void(*handleUseItem_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, UseItemPacket *);
	static void handleUseItem(ServerNetworkHandler *, const RakNet::RakNetGUID &, UseItemPacket *);

	static void(*handlePlayerAction_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, PlayerActionPacket *);
	static void handlePlayerAction(ServerNetworkHandler *, const RakNet::RakNetGUID&, PlayerActionPacket *);

	static void(*handleDropItem_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, DropItemPacket *);
	static void handleDropItem(ServerNetworkHandler *, const RakNet::RakNetGUID &, DropItemPacket *);

	static void(*handleContainerClose_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerClosePacket *);
	static void handleContainerClose(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerClosePacket *);

	static void(*handleContainerSetSlot_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerSetSlotPacket *);
	static void handleContainerSetSlot(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerSetSlotPacket *);

	static void(*handleContainerSetContent_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerSetContentPacket *);
	static void handleContainerSetContent(ServerNetworkHandler *, const RakNet::RakNetGUID &, ContainerSetContentPacket *);

	static void(*handleCraftingEvent_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, CraftingEventPacket *);
	static void handleCraftingEvent(ServerNetworkHandler *, const RakNet::RakNetGUID &, CraftingEventPacket *);

	static void(*handleAnimate_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, AnimatePacket *);
	static void handleAnimate(ServerNetworkHandler *, const RakNet::RakNetGUID &, AnimatePacket *);

	static void(*handleBlockEntityData_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, BlockEntityDataPacket *);
	static void handleBlockEntityData(ServerNetworkHandler *, const RakNet::RakNetGUID &, BlockEntityDataPacket *);

	static void(*handlePlayerInput_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, PlayerInputPacket *);
	static void handlePlayerInput(ServerNetworkHandler *, const RakNet::RakNetGUID &, PlayerInputPacket *);

	static void(*handleSpawnExperienceOrb_real)(ServerNetworkHandler *, const RakNet::RakNetGUID &, SpawnExperienceOrbPacket *);
	static void handleSpawnExperienceOrb(ServerNetworkHandler *, const RakNet::RakNetGUID &, SpawnExperienceOrbPacket *);

	static void(*allowIncomingConnections_real)(ServerNetworkHandler *, const std::string &, bool);
	static void allowIncomingConnections(ServerNetworkHandler *, const std::string &, bool);

	static void setupHooks();
};
