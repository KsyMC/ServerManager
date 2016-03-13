#pragma once

class ServerOperator
{
public:
	virtual ~ServerOperator() {}

	virtual bool isOp() const = 0;
	virtual void setOp(bool value) = 0;

	virtual bool isPermissible() { return false; }
};