#include "BlockEvent.h"

BlockEvent::BlockEvent(Block *theBlock)
{
	block = theBlock;
}

Block *BlockEvent::getBlock() const
{
	return block;
}
