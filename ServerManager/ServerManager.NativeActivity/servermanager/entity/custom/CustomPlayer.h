#pragma once

class Player;
class ItemInstance;
class Entity;
class BlockPos;

class CustomPlayer
{
public:
	static void (*drop_real)(Player *, ItemInstance *, bool);
	static void drop(Player *, ItemInstance *, bool);

	static int (*startSleepInBed_real)(Player *, const BlockPos &);
	static int startSleepInBed(Player *, const BlockPos &);

	static void (*stopSleepInBed_real)(Player *, bool, bool);
	static void stopSleepInBed(Player *, bool, bool);

	static void setupHooks();
};
