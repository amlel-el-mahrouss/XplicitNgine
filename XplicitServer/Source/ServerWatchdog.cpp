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

namespace Xplicit
{
	static int16_t XPLICIT_WATCHDOG_DELAY = 100000;

	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog(XPLICIT_WATCHDOG_DELAY)
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
				if (m_watchdog == XPLICIT_WATCHDOG_DELAY)
				{
					for (size_t i = 0; i < server->size(); i++)
					{
						if (server->get(i)->stat == NETWORK_STAT_DISCONNECTED)
							continue;

						if (server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN ||
							server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
							continue;

						server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_WATCHDOG] = NETWORK_CMD_WATCHDOG;
					}
				}

				--this->m_watchdog;
			}
			else
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i)->stat == NETWORK_STAT_DISCONNECTED)
						continue;

					if (server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
					{
						server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
						XPLICIT_INFO("[WATCHDOG] " + uuids::to_string(server->get(i)->unique_addr.uuid));
					}
				}

				this->m_watchdog = XPLICIT_WATCHDOG_DELAY;
			}
		}
	}

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }
}