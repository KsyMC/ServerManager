#pragma once

class Creeper;

class CustomCreeper
{
public:
	static void(*onLightningHit_real)(Creeper *);
	static void onLightningHit(Creeper *);

	static void setupHooks();
};
