#pragma once

#include <raknet/RakNetTypes.h>

#include "minecraftpe/gamemode/GameType.h"
#include "minecraftpe/util/UUID.h"

class LocalPlayer;
class MinecraftClient;
class Level;
class User;

class CustomLocalPlayer
{
public:
	static void(*constructor_real)(LocalPlayer *, MinecraftClient *, Level &, const User &, GameType, const RakNet::RakNetGUID &, mce::UUID);
	static void constructor(LocalPlayer *, MinecraftClient *, Level &, const User &, GameType, const RakNet::RakNetGUID &, mce::UUID);

	static void setupHooks();
};
