#pragma once

#include "SMAnimal.h"

class MushroomCow;

class SMMushroomCow : public SMAnimal
{
public:
	SMMushroomCow(Server *server, MushroomCow *entity);

	MushroomCow *getHandle() const;
	void setHandle(MushroomCow *entity);
};
