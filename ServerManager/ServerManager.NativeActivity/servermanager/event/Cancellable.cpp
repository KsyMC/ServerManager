#include "Cancellable.h"

Cancellable::~Cancellable()
{
}

bool Cancellable::isCancellable() const
{
	return true;
}
