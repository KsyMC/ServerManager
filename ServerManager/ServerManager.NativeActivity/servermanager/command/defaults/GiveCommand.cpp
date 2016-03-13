#include "GiveCommand.h"
#include "../../ServerManager.h"
#include "../../entity/player/SMPlayer.h"
#include "../../level/SMBlockSource.h"
#include "../../util/SMUtil.h"
#include "../../ChatColor.h"
#include "minecraftpe/world/entity/player/Player.h"
#include "minecraftpe/world/item/Item.h"
#include "minecraftpe/world/item/ItemInstance.h"
#include "minecraftpe/world/level/block/Block.h"
#include "minecraftpe/world/phys/Vec3.h"

GiveCommand::GiveCommand()
	: VanillaCommand("give")
{
	description = "Gives the specified player a certain amount of items";
	usageMessage = "%commands.give.usage";
	setPermission("servermanager.command.give");
}

bool GiveCommand::execute(CommandSender *sender, std::string &label, std::vector<std::string> &args)
{
	if (!testPermission(sender)) return true;
	if ((int)args.size() < 2)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.usage", { usageMessage });
		return false;
	}

	SMPlayer *player = ServerManager::getPlayer(args[0]);
	if (!player)
	{
		sender->sendTranslation(ChatColor::RED + "%commands.generic.player.notFound", {});
		return true;
	}

	std::string name = args[1];
	std::string prefix = "minecraft:";
	if (name.find(prefix) != std::string::npos)
		name.replace(name.find(prefix), prefix.length(), "");

	int id;

	Item *item = Item::lookupByName(name, true);
	if (!item)
	{
		Block *block = Block::lookupByName(name, true);
		if (!block)
		{
			if (!SMUtil::is_number(name))
			{
				sender->sendTranslation(ChatColor::RED + "%commands.give.item.notFound", { args[1] });
				return true;
			}
			else
				id = SMUtil::toInt(name);
		}
		else
			id = block->id;
	}
	else
		id = item->id;

	int count = 0;
	int auxValue = 0;

	if ((int)args.size() > 2 && SMUtil::toInt(args[2]))
		count = SMUtil::toInt(args[2]);

	if ((int)args.size() > 3 && SMUtil::toInt(args[3]))
		auxValue = SMUtil::toInt(args[3]);

	if ((int)args.size() > 4)// tag
	{
		sender->sendTranslation("commands.give.tagError", { "Invalid tag conversion" });
	}

	ItemInstance itemInst(id, count, auxValue);
	if (itemInst.isNull())
	{
		sender->sendTranslation(ChatColor::RED + "%commands.give.item.notFound", { args[1] });
		return true;
	}

	if (count == 0)
	{
		count = itemInst.getMaxStackSize();
		itemInst.set(count);
	}

	player->getRegion()->dropItem(player->getHandle()->getPos(), itemInst, 0);

	Command::broadcastCommandTranslation(sender, "commands.give.success", {
			itemInst.getName() + " (" + SMUtil::toString(itemInst.getId()) + ":" + SMUtil::toString(itemInst.getAuxValue()) + ")",
			SMUtil::toString(count), player->getName() });

	return true;
}
