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
		: Event(), m_watchdog_analyze(false), m_watchdog_enabled(false) 
	{}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		if (!this->m_watchdog_enabled)
			return;

		auto server = InstanceManager::get_singleton_ptr()->find<NetworkServerInstance>("NetworkServerInstance");

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
					if (server->get(i).packet.CMD != NETWORK_CMD_ACK)
					{
						server->get(i).stat = NETWORK_STAT_DISCONNECTED;
					}
				}

				this->m_watchdog_enabled = false;
				this->m_watchdog_analyze = true;
			}
		}
	}

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }

	ServerWatchdogEvent::operator bool() { return m_watchdog_enabled; }

	void ServerWatchdogEvent::set(bool enable) noexcept
	{
		m_watchdog_enabled = enable;
	}
}