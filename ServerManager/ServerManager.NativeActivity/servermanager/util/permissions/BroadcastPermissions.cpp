#include "BroadcastPermissions.h"
#include "DefaultPermissions.h"
#include "../../permissions/Permission.h"

const std::string BroadcastPermissions::ROOT = "servermanager.broadcast";
const std::string BroadcastPermissions::PREFIX = ROOT + ".";

Permission *BroadcastPermissions::registerPermissions(Permission *parent)
{
	Permission *broadcasts = DefaultPermissions::registerPermission(ROOT, "Allows the user to receive all broadcast messages", parent);
	DefaultPermissions::registerPermission(PREFIX + "admin", "Allows the user to receive administrative broadcasts", PermissionDefault::OP, broadcasts);
	DefaultPermissions::registerPermission(PREFIX + "user", "Allows the user to receive user broadcasts", PermissionDefault::TRUE, broadcasts);

	broadcasts->recalculatePermissibles();

	return broadcasts;
}