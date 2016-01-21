#pragma once

#include "SMMob.h"

class Blaze;

class SMBlaze : public SMMob
{
public:
	SMBlaze(Server *server, Blaze *entity);

	Blaze *getHandle() const;
	void setHandle(Blaze *entity);
};
