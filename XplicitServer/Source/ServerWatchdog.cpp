/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: ServerWatchdog.cpp
 *			Purpose: Server Watchdog
 *
 * =====================================================================
 */

#include "ServerWatchdog.h"
#include "Actor.h"

namespace Xplicit
{
	static int16_t XPLICIT_WATCHDOG_DELAY = 1000;

	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog(0)
	{
	}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (server)
		{
			if (this->m_watchdog > 0)
			{
				if (m_watchdog >= XPLICIT_WATCHDOG_DELAY)
				{
					for (size_t i = 0; i < server->size(); i++)
					{
						server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_WATCHDOG] = NETWORK_CMD_WATCHDOG;
					}
				}

				--this->m_watchdog;
			}
			else
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
						server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
				}

				this->m_watchdog = XPLICIT_WATCHDOG_DELAY;
			}
		}
	}

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }

	void ServerWatchdogEvent::watchdog() noexcept
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");
		
		if (!server)
			return;

		for (size_t i = 0; i < server->size(); i++)
		{
			if (server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
			{
				server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
			}
		}
	}
}