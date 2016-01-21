#pragma once

class ItemEntity;
class Player;

class CustomItemEntity
{
public:
	static void(*playerTouch_real)(ItemEntity *, Player &);
	static void playerTouch(ItemEntity *, Player &);

	static void setupHooks();
};
