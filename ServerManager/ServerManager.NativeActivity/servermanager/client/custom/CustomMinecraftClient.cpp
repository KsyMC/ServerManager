#include <dlfcn.h>

#include "CustomMinecraftClient.h"
#include "../../ServerManager.h"
#include "minecraftpe/client/AppPlatform.h"
#include "minecraftpe/client/MinecraftClient.h"
#include "Substrate.h"

void(*CustomMinecraftClient::init_real)(MinecraftClient *client);
void CustomMinecraftClient::init(MinecraftClient *client)
{
	init_real(client);
	ServerManager::getServer()->init(client->getServer(), AppPlatform::mSingleton->getUserdataPath() + client->dataDir);
}

void(*CustomMinecraftClient::leaveGame_real)(MinecraftClient *client, bool b1);
void CustomMinecraftClient::leaveGame(MinecraftClient *client, bool b1)
{
	ServerManager::getServer()->stop();
	leaveGame_real(client, b1);
}

void(*CustomMinecraftClient::destructor_real)(MinecraftClient *client);
void CustomMinecraftClient::destructor(MinecraftClient *client)
{
	ServerManager::deleteServer();
	destructor_real(client);
}

void CustomMinecraftClient::setupHooks()
{
	MSHookFunction((void *)&MinecraftClient::init, (void *)&init, (void **)&init_real);
	MSHookFunction((void *)&MinecraftClient::leaveGame, (void *)&leaveGame, (void **)&leaveGame_real);
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN15MinecraftClientD2Ev"), (void *)&destructor, (void **)&destructor_real);
}
