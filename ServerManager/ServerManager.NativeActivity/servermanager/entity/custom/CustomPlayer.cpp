#include "CustomPlayer.h"
#include "../../ServerManager.h"
#include "../../event/player/PlayerDropItemEvent.h"
#include "../../event/player/PlayerBedEnterEvent.h"
#include "../../event/player/PlayerBedLeaveEvent.h"
#include "../../plugin/PluginManager.h"
#include "minecraftpe/world/entity/player/Player.h"
#include "minecraftpe/world/entity/player/Inventory.h"
#include "minecraftpe/world/level/Level.h"
#include "minecraftpe/world/level/Dimension.h"
#include "minecraftpe/world/level/BlockSource.h"
#include "minecraftpe/world/level/block/Block.h"
#include "Substrate.h"

void(*CustomPlayer::drop_real)(Player *real, ItemInstance *item, bool b);
void CustomPlayer::drop(Player *real, ItemInstance *item, bool b)
{
	if (!real->level->isClientSide())
	{
		SMPlayer *smPlayer = ServerManager::getServer()->getPlayer(real);

		PlayerDropItemEvent event(smPlayer, item);
		ServerManager::getPluginManager()->callEvent(event);

		if (event.isCancelled())
		{
			if (!smPlayer->isLocalPlayer())
				real->sendInventory();
			else
				real->inventory->add(*item);
			return;
		}
	}
	drop_real(real, item, b);
}

int(*CustomPlayer::startSleepInBed_real)(Player *real, const BlockPos &pos);
int CustomPlayer::startSleepInBed(Player *real, const BlockPos &pos)
{
	if (!real->level->isClientSide())
	{
		if (real->region)
		{
			if (real->isSleeping() || !real->isAlive())
				return 4; // other problem

			if (fabs(real->pos.x - pos.x) > 3.0f || fabs(real->pos.y - pos.y) > 2.0f || fabs(real->pos.z - pos.z) > 3.0f)
				return 3; // too far away

			if (!real->getDimension()->isNaturalDimension())
				return 1; // not possible here

			if (real->getDimension()->isDay())
				return 2; // not night

			AABB area(pos.x - 8.0f, pos.y - 5.0f, pos.z - 8.0f, pos.x + 8.0f, pos.y + 5.0f, pos.z + 8.0f);
			std::vector<Entity *> list = real->region->getEntities(EntityType::MONSTER, area, NULL);

			if (!list.empty())
				return 5; // not safe
		}

		FullBlock block = real->region->getBlockAndData(pos);
		PlayerBedEnterEvent event(ServerManager::getServer()->getPlayer(real), block);
		ServerManager::getPluginManager()->callEvent(event);

		if (event.isCancelled())
			return 4; // other problem
	}
	return startSleepInBed_real(real, pos);
}

void(*CustomPlayer::stopSleepInBed_real)(Player *real, bool b1, bool b2);
void CustomPlayer::stopSleepInBed(Player *real, bool b1, bool b2)
{
	if (!real->level->isClientSide())
	{
		if (!real->isSleeping())
			return;

		if (real->region->getBlockID(real->bedPosition) != Block::mBed->id)
			return;

		FullBlock bed = real->getRegion()->getBlockAndData(real->bedPosition);

		PlayerBedLeaveEvent event(ServerManager::getServer()->getPlayer(real), bed);
		ServerManager::getPluginManager()->callEvent(event);
	}
	stopSleepInBed_real(real, b1, b2);
}

void CustomPlayer::setupHooks()
{
	MSHookFunction(dlsym(RTLD_DEFAULT, "_ZN6Player4dropEPK12ItemInstanceb"), (void *)&drop, (void **)&drop_real);
	//MSHookFunction((void *)&Player::startSleepInBed, (void *)&startSleepInBed, (void **)&startSleepInBed_real);
	//MSHookFunction((void *)&Player::stopSleepInBed, (void *)&stopSleepInBed, (void **)&stopSleepInBed_real);
}
