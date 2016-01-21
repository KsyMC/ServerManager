#pragma once

#include "../Event.h"

class Block;

class BlockEvent : public Event
{
protected:
	Block *block;

public:
	BlockEvent(Block *theBlock);

	Block *getBlock() const;
};
