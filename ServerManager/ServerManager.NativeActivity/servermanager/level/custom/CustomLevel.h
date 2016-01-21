#pragma once

#include <memory>

#include "minecraftpe/entity/Entity.h"

class Level;
class Entity;

class CustomLevel
{
public:
	static void (*removeEntity_real)(Level *, Entity *, bool);
	static void removeEntity(Level *, Entity *, bool);

	static void setupHooks();
};
