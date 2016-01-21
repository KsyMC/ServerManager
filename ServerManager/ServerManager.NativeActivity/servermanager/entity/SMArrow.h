#pragma once

#include "SMEntity.h"

class Arrow;

class SMArrow : public SMEntity
{
public:
	SMArrow(Server *server, Arrow *entity);

	Arrow *getHandle() const;
	void setHandle(Arrow *entity);
};
