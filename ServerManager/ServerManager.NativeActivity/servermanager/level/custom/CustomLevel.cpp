#include <dlfcn.h>

#include "CustomLevel.h"
#include "../../ServerManager.h"
#include "minecraftpe/level/Level.h"
#include "Substrate.h"

void(*CustomLevel::removeEntity_real)(Level *real, Entity *entity, bool b);
void CustomLevel::removeEntity(Level *real, Entity *entity, bool b)
{
	ServerManager::getServer()->removeEntity(entity);
	removeEntity_real(real, entity, b);
}

void CustomLevel::setupHooks()
{
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN5Level12removeEntityER6Entityb"), (void *)&removeEntity, (void **)&removeEntity_real);
}
