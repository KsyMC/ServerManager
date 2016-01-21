#pragma once

class Arrow;
class Player;

class CustomArrow
{
public:
	static void (*playerTouch_real)(Arrow *, Player &);
	static void playerTouch(Arrow *, Player &);

	static void setupHooks();
};
