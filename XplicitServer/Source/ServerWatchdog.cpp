/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: ServerWatchdog.cpp
 *			Purpose: Server Watchdog
 *
 * =====================================================================
 */

#include "ServerWatchdog.h"

namespace Xplicit
{
	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog_analyze(false)
	{}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (server)
		{
			if (this->m_watchdog_analyze)
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					server->get(i).packet.CMD = NETWORK_CMD_WATCHDOG;
				}

				this->m_watchdog_analyze = false;
			}
			else
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i).packet.CMD == NETWORK_CMD_INVALID)
					{
						server->get(i).stat = NETWORK_STAT_DISCONNECTED;
					}
					else if (server->get(i).packet.CMD == NETWORK_CMD_BEGIN)
					{
						server->get(i).stat = NETWORK_STAT_CONNECTING;
					}
					else
					{
						server->get(i).stat = NETWORK_STAT_CONNECTED;

					}
				}

				this->m_watchdog_analyze = true;
			}
		}
	}

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }
}