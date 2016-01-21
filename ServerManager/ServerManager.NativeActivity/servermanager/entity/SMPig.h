#pragma once

#include "SMAnimal.h"

class Pig;

class SMPig : public SMAnimal
{
public:
	SMPig(Server *server, Pig *entity);

	Pig *getHandle() const;
	void setHandle(Pig *entity);
};
