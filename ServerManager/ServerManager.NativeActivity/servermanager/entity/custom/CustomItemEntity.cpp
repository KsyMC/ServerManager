#include "CustomItemEntity.h"
#include "../../ServerManager.h"
#include "../../event/player/PlayerPickupItemEvent.h"
#include "../../plugin/PluginManager.h"
#include "minecraftpe/world/entity/item/ItemEntity.h"
#include "minecraftpe/world/entity/player/Inventory.h"
#include "minecraftpe/world/level/Level.h"
#include "Substrate.h"

void(*CustomItemEntity::playerTouch_real)(ItemEntity *real, Player &player);
void CustomItemEntity::playerTouch(ItemEntity *real, Player &player)
{
	if (!real->level->isClientSide())
	{
		int canHold = real->item.count;/*player.inventory->_getSlotWithRemainingSpace(real->item);*/
		int remaining = real->item.count - canHold;

		if (!player.isAlive() || real->pickupDelay > 0 || canHold <= 0 || !player.inventory->canAdd(real->item))
			return;

		real->item.count = canHold;
		PlayerPickupItemEvent event(ServerManager::getServer()->getPlayer(&player), &real->item, remaining);
		ServerManager::getPluginManager()->callEvent(event);
		real->item.count = canHold + remaining;

		if (event.isCancelled())
			return;
	}
	playerTouch_real(real, player);
}

void CustomItemEntity::setupHooks()
{
	MSHookFunction((void *)&ItemEntity::playerTouch, (void *)&playerTouch, (void **)&playerTouch_real);
}
