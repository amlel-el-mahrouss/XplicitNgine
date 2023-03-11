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
	static int16_t XPLICIT_WATCH_DELAY = 5000;

	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog_delay(0)
	{
	}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (server)
		{
			if (this->m_watchdog_delay > 0)
			{
				--this->m_watchdog_delay;
			}
			else
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					server->get(i).packet.CMD = NETWORK_CMD_WATCHDOG;
				}

				this->m_watchdog_delay = XPLICIT_WATCH_DELAY;
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
			// ignore, incoming, disconnecting connections, alongside scripts.
			if (server->get(i).packet.CMD == NETWORK_CMD_BEGIN ||
				server->get(i).packet.CMD == NETWORK_CMD_STOP ||
				server->get(i).packet.CMD == NETWORK_CMD_SCRIPT)
				continue;

			if (server->get(i).packet.CMD != NETWORK_CMD_ACK)
			{
				server->get(i).packet.CMD = NETWORK_CMD_KICK;
			}
		}
	}
}