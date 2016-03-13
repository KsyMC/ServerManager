#pragma once

class PermissionAttachment;

class PermissionRemovedExecutor
{
public:
	virtual void attachmentRemoved(PermissionAttachment *attachment) = 0;
};