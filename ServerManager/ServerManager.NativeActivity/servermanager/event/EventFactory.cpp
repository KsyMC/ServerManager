#include <memory>
#include <string>

#include "../ServerManager.h"
#include "../block/SMBlock.h"
#include "../entity/SMPlayer.h"
#include "EventFactory.h"
#include "../plugin/PluginManager.h"
#include "minecraftpe/item/ItemInstance.h"
#include "minecraftpe/entity/player/Player.h"
#include "minecraftpe/level/BlockSource.h"

std::unique_ptr<PlayerInteractEvent> EventFactory::callPlayerInteractEvent(Player *who, Action action, ItemInstance *itemInHand)
{
	if(action != Action::LEFT_CLICK_AIR && action != Action::RIGHT_CLICK_AIR)
		return std::unique_ptr<PlayerInteractEvent>();
	return std::move(callPlayerInteractEvent(who, action, 0, 256, 0, 0, itemInHand));
}

std::unique_ptr<PlayerInteractEvent> EventFactory::callPlayerInteractEvent(Player *who, Action action, int clickedX, int clickedY, int clickedZ, int clickedFace, ItemInstance *itemInHand)
{
	SMPlayer *player = !who ? NULL : ServerManager::getServer()->getPlayer(who);

	Block *blockClicked = who->getRegion()->getBlock(clickedX, clickedY, clickedZ);
	BlockFace blockFace = SMBlock::notchToBlockFace(clickedFace);

	if(clickedY > 255)
	{
		blockClicked = NULL;
		switch(action)
		{
		case Action::LEFT_CLICK_BLOCK:
			action = Action::LEFT_CLICK_AIR;
			break;
		case Action::RIGHT_CLICK_BLOCK:
			action = Action::RIGHT_CLICK_AIR;
			break;
		}
	}

	if(itemInHand->getId() == 0 || itemInHand->count == 0)
		itemInHand = NULL;

	std::unique_ptr<PlayerInteractEvent> event = std::unique_ptr<PlayerInteractEvent>(new PlayerInteractEvent(player, action, itemInHand, blockClicked, blockFace));
	player->getServer()->getPluginManager()->callEvent(*event.get());

	return event;
}
