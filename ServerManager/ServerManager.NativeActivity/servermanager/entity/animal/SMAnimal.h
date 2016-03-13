#pragma once

#include "../SMAgableMob.h"

class Animal;

class SMAnimal : public SMAgableMob
{
public:
	SMAnimal(Server *server, Animal *entity);

	Animal *getHandle() const;
	void setHandle(Animal *entity);
};
