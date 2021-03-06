#pragma once

#include "SMAnimal.h"

class Cow;

class SMCow : public SMAnimal
{
public:
	SMCow(Server *server, Cow *entity);

	Cow *getHandle() const;
	void setHandle(Cow *entity);
};
