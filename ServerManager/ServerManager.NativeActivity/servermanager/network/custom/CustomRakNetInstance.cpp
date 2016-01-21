#include "CustomRakNetInstance.h"
#include "../../ServerManager.h"
#include "minecraftpe/network/RakNetInstance.h"
#include "Substrate.h"

void(*CustomRakNetInstance::_startupIfNeeded_real)(RakNetInstance *real, unsigned short port, int connections);
void CustomRakNetInstance::_startupIfNeeded(RakNetInstance *real, unsigned short port, int connections)
{
	_startupIfNeeded_real(real, ServerManager::getServer()->getPort(), ServerManager::getMaxPlayers());
}

void(*CustomRakNetInstance::host_real)(RakNetInstance *real, const std::string &name, int port, int connections);
void CustomRakNetInstance::host(RakNetInstance *real, const std::string &name, int port, int connections)
{
	host_real(real, name, ServerManager::getPort(), ServerManager::getMaxPlayers());
}

void CustomRakNetInstance::setupHooks()
{
	MSHookFunction((void *) &RakNetInstance::_startupIfNeeded, (void **) &_startupIfNeeded, (void **) &_startupIfNeeded_real);
	MSHookFunction((void *) &RakNetInstance::host, (void **) &host, (void **) &host_real);
}
