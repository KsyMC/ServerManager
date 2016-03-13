#include "CustomCreeper.h"
#include "../../ServerManager.h"
#include "../../event/entity/CreeperPowerEvent.h"
#include "../../plugin/PluginManager.h"
#include "minecraftpe/world/entity/monster/Creeper.h"
#include "minecraftpe/world/level/Level.h"
#include "Substrate.h"

void(*CustomCreeper::onLightningHit_real)(Creeper *real);
void CustomCreeper::onLightningHit(Creeper *real)
{
	if (!real->level->isClientSide())
	{
		CreeperPowerEvent event((SMCreeper *)ServerManager::getEntity(real), CreeperPowerEvent::LIGHTNING);
		ServerManager::getPluginManager()->callEvent(event);

		if (event.isCancelled())
			return;
	}
	real->_setPowered(true);
}

void CustomCreeper::setupHooks()
{
	MSHookFunction((void *)&Creeper::onLightningHit, (void *)&onLightningHit, (void **)&onLightningHit_real);
}
