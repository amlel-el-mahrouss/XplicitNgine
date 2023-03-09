/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event
 *
 * =====================================================================
 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	static void xplicit_join_event(NetworkClient& cl, 
		Actor* actor, 
		NetworkServerInstance* server, 
		int64_t& counter)
	{
		actor->set(cl.addr);
		actor->set(counter);

		cl.packet.CMD = NETWORK_CMD_ACCEPT;
		cl.packet.ID = actor->id();

		server->send();

		// force update
		auto env = EventDispatcher::get_singleton_ptr()->get<NetworkServerEvent>("NetworkServerEvent");
		XPLICIT_NET_ASSERT(env);

		env->update();

		++counter;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : m_id_counter(0) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() {}

	void PlayerJoinLeaveEvent::operator()()
	{
		// the main logic for the join and leave event.
		// here we simply handle player logic.

		this->on_join();
		this->on_leave();

	}

	bool PlayerJoinLeaveEvent::on_join() noexcept
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server) 
			return false;

		for (size_t i = 0; i < server->size(); i++)
		{
			if (server->get(i).packet.CMD == NETWORK_CMD_BEGIN)
			{
				auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

				for (size_t y = 0; y < actors.size(); y++)
				{
					if (actors[y]->get().sockaddr.sin_addr.S_un.S_addr == server->get(i).addr.sin_addr.S_un.S_addr)
						return false;

					if (actors[y]->get().sockaddr.sin_addr.S_un.S_addr == 0)
					{
						xplicit_join_event(server->get(i), actors[y], server, m_id_counter);

						XPLICIT_INFO("[ACTOR] Actor.Update");

						return true;
					}
				}

				auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
					throw std::runtime_error("OutOfMemory: Could not allocate further actors!");

				xplicit_join_event(server->get(i), actor, server, m_id_counter);

				XPLICIT_INFO("[ACTOR] Actor.New");

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
			if (server->get(i).packet.CMD == NETWORK_CMD_STOP)
			{
				auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

				for (size_t y = 0; y < actors.size(); y++)
				{
					if (actors[y]->get().sockaddr.sin_addr.S_un.S_addr == server->get(i).addr.sin_addr.S_un.S_addr)
					{
						actors[y]->reset();
						server->get(i).reset();

						// decrement the entity counter
						--m_id_counter;

#ifndef _NDEBUG
						XPLICIT_INFO("[ACTOR] Actor.Remove");
#endif

						return true;
					}
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}