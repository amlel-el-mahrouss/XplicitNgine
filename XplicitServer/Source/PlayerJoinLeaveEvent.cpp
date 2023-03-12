/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.cpp
 *			Purpose: Player Join and Leave logic
 *
 * =====================================================================
 */

#include "PlayerJoinLeaveEvent.h"
#include "Actor.h"

namespace Xplicit
{
	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() 
		: m_actor_counter(0) 
	{

	}

	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() 
	{
	}

	int64_t PlayerJoinLeaveEvent::size() noexcept
	{
		return m_actor_counter;
	}

	void PlayerJoinLeaveEvent::operator()()
	{
		// the main logic for the join and leave event.
		// here we simply handle player logic.

		this->on_leave();
		this->on_join();
	}

	bool PlayerJoinLeaveEvent::on_join() noexcept
	{
		if (m_actor_counter > MAX_CONNECTIONS)
			return false;

		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");
		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		if (!server) 
			return false;

		for (size_t i = 0; i < server->size(); i++)
		{
			for (size_t y = 0; y < actors.size(); y++)
			{
				if (equals(actors[y]->get().addr, server->get(i).addr))
				{
					server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					server->get(i).stat = NETWORK_STAT_DISCONNECTED;

					return false;
				}
			}

			if (server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN)
			{
				auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
					return false;

				actor->set(server->get(i).addr);

				server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				server->get(i).stat = NETWORK_STAT_CONNECTED;

				server->get(i).packet.ID = m_actor_counter;
				actor->get().id = m_actor_counter;

				++m_actor_counter;

				return true;
			}
		}

		return false;
	}

	bool PlayerJoinLeaveEvent::on_leave() noexcept
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server) 
			return false;

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

						--m_actor_counter;

						return true;
					}
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}