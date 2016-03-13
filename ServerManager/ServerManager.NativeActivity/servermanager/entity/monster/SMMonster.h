#pragma once

#include "../SMPathfinderMob.h"

class Monster;

class SMMonster : public SMPathfinderMob
{
public:
	SMMonster(Server *server, Monster *entity);

	Monster *getHandle() const;
	void setHandle(Monster *entity);
};
