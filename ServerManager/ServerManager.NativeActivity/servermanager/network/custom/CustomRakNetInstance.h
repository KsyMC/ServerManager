#pragma once

#include <string>

class RakNetInstance;

class CustomRakNetInstance
{
public:
	static void(*_startupIfNeeded_real)(RakNetInstance *, unsigned short port, int connections);
	static void _startupIfNeeded(RakNetInstance *, unsigned short port, int connections);

	static void(*host_real)(RakNetInstance *, const std::string &name, int port, int connections);
	static void host(RakNetInstance *, const std::string &name, int port, int connections);

	static void setupHooks();
};
