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
	PlayerManager::PlayerManager()
		: m_players(0), m_host()
	{
		for (size_t actor = 0; actor < MAX_CONNECTIONS; actor++)
		{
			auto actor_ptr = InstanceManager::get_singleton_ptr()->add<Actor>();
			assert(actor_ptr);

			actor_ptr->reset();
		}
	}

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

		for (size_t server_index = 0; server_index < server->size(); server_index++)
		{
			if (server->get(server_index).packet.CMD[XPLICIT_NETWORK_CMD_BEGIN] != NETWORK_CMD_BEGIN)
				continue;

			for (size_t actor_index = 0; actor_index < actors.size(); actor_index++)
			{
				if (equals(actors[actor_index]->get().addr, server->get(server_index).addr))
				{
					server->get(server_index).packet.CMD[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					server->get(server_index).stat = NETWORK_STAT_DISCONNECTED;

					return;
				}

				if (actors[actor_index]->get().addr.sin_addr.S_un.S_addr == 0)
				{
					actors[actor_index]->set(server->get(server_index).addr);

					server->get(server_index).packet.CMD[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
					server->get(server_index).stat = NETWORK_STAT_CONNECTED;
					server->get(server_index).packet.ID = PlayerManager::get_singleton_ptr()->size();

					actors[actor_index]->get().id = server->get(server_index).packet.ID;

					++(*PlayerManager::get_singleton_ptr());

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("Actor.New");
#endif // XPLICIT_DEBUG

				}
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
						--(*PlayerManager::get_singleton_ptr());

						server->get(i).reset();
						actors[y]->reset();

#ifdef XPLICIT_DEBUG
						XPLICIT_INFO("Actor.Delete");
#endif // XPLICIT_DEBUG

						break;
					}
				}
			}
		}
	}

	const char* PlayerLeaveEvent::name() noexcept { return ("PlayerLeaveEvent"); }

}