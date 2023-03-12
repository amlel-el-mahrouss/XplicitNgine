/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerEvent.cpp
 *			Purpose: Player Events
 *
 * =====================================================================
 */

#include "PlayerEvent.h"
#include "Actor.h"

namespace Xplicit
{
	std::unique_ptr<PlayerManager>& PlayerManager::get_singleton_ptr() noexcept
	{
		static std::unique_ptr<PlayerManager> manager = std::unique_ptr<PlayerManager>(new PlayerManager());
		return manager;
	}

	const int64_t& PlayerManager::size() noexcept { return m_players; }

	void PlayerJoinEvent::operator()()
	{
		if (PlayerManager::get_singleton_ptr()->size() > MAX_CONNECTIONS)
			return;

		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		for (size_t i = 0; i < server->size(); i++)
		{
			for (size_t y = 0; y < actors.size(); y++)
			{
				if (equals(actors[y]->get().addr, server->get(i).addr))
				{
					server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					server->get(i).stat = NETWORK_STAT_DISCONNECTED;

					return;
				}
			}

			if (server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN)
			{
				auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
					throw EngineError();

				actor->set(server->get(i).addr);

				server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				server->get(i).stat = NETWORK_STAT_CONNECTED;
				server->get(i).packet.ID = PlayerManager::get_singleton_ptr()->size();

				actor->get().id = server->get(i).packet.ID;

				XPLICIT_INFO("Ngin");

				++*PlayerManager::get_singleton_ptr();
			}
		}
	}

	const char* PlayerJoinEvent::name() noexcept { return ("PlayerJoinEvent"); }

	void PlayerLeaveEvent::operator()()
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		for (size_t i = 0; i < server->size(); i++)
		{
			if (server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

				for (size_t y = 0; y < actors.size(); y++)
				{
					if (equals(actors[y]->get().addr, server->get(i).addr))
					{
						server->get(i).reset();

						InstanceManager::get_singleton_ptr()->remove<Actor>(actors[y]);
						--*PlayerManager::get_singleton_ptr();

						XPLICIT_INFO("Xplicit");

						break;
					}
				}
			}
		}
	}

	const char* PlayerLeaveEvent::name() noexcept { return ("PlayerLeaveEvent"); }

}