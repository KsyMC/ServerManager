#pragma once

class MinecraftClient;

class CustomMinecraftClient
{
public:
	static void (*init_real)(MinecraftClient *);
	static void init(MinecraftClient *);

	static void (*leaveGame_real)(MinecraftClient *, bool);
	static void leaveGame(MinecraftClient *, bool);

	static void (*destructor_real)(MinecraftClient *);
	static void destructor(MinecraftClient *);

	static void setupHooks();
};
