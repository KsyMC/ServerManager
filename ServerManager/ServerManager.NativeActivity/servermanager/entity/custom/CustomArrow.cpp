#include "CustomArrow.h"
#include "../../ServerManager.h"
#include "../../event/player/PlayerPickupItemEvent.h"
#include "../../plugin/PluginManager.h"
#include "minecraftpe/entity/Arrow.h"
#include "minecraftpe/entity/player/Inventory.h"
#include "minecraftpe/level/Level.h"
#include "minecraftpe/item/Item.h"
#include "Substrate.h"

void(*CustomArrow::playerTouch_real)(Arrow *real, Player &player);
void CustomArrow::playerTouch(Arrow *real, Player &player)
{
	if(!real->level->isClientSide() && real->getEnchantInfinity() == 0)
	{
		ItemInstance arrowItem(Item::mArrow, 1);
		if(!real->onGround || !real->fromPlayer || real->shakeTime > 0 || player.inventory->canAdd(arrowItem))
			return;

		PlayerPickupItemEvent event(ServerManager::getServer()->getPlayer(&player), &arrowItem, 0);
		ServerManager::getPluginManager()->callEvent(event);

		if(event.isCancelled())
			return;
	}
	playerTouch_real(real, player);
}

void CustomArrow::setupHooks()
{
	MSHookFunction((void *)&Arrow::playerTouch, (void *)&playerTouch, (void **)&playerTouch_real);
}
