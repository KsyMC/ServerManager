#pragma once

#include "../SMEntity.h"

class Boat;

class SMBoat : public SMEntity
{
public:
	SMBoat(Server *server, Boat *entity);

	Boat *getHandle() const;
	void setHandle(Boat *entity);
};
