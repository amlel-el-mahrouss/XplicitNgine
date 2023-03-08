/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX Technologies all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event
 *
 * =====================================================================
 */

#include "PlayerJoinLeaveEvent.h"

namespace Xplicit
{
	static void xplicit_join_event(NetworkClient cl, ActorInstance* actor, NetworkServerInstance* server, int64_t counter)
	{
		actor->set(cl.addr);
		actor->set(counter);

		cl.packet.CMD = NETWORK_CMD_ACCEPT;
		cl.packet.ID = actor->id();

		server->send();
	}

	bool PlayerJoinLeaveEvent::on_join() noexcept
	{
		// check for the instance manager.
		if (!InstanceManager::get_singleton_ptr())
			return false;

		auto server_event = EventDispatcher::get_singleton_ptr()->find<NetworkServerEvent>("NetworkServerEvent");
		if (!server_event) return false;

		auto server = InstanceManager::get_singleton_ptr()->find<NetworkServerInstance>("NetworkServerInstance");
		if (!server) return false;

		for (size_t i = 0; i < server->size(); i++)
		{
			auto cl = server->get(i);

			if (cl.packet.CMD == NETWORK_CMD_BEGIN)
			{
				auto actors = InstanceManager::get_singleton_ptr()->find_all<ActorInstance>("ActorInstance");

				for (size_t y = 0; y < actors.size(); y++)
				{
					if (actors[y]->sockaddr().sin_addr.S_un.S_addr == cl.addr.sin_addr.S_un.S_addr)
						return false;

					if (actors[y]->sockaddr().sin_addr.S_un.S_addr == 0)
					{
						xplicit_join_event(cl, actors[y], server, m_id_counter);
						++m_id_counter;

						XPLICIT_INFO("Reused exisiting actor..");

						return true;
					}
				}

				auto actor = InstanceManager::get_singleton_ptr()->add<ActorInstance>();

				if (!actor)
					throw std::runtime_error("OutOfMemory: Could not allocate further actors!");

				xplicit_join_event(cl, actor, server, m_id_counter);
				++m_id_counter;

				return true;
			}
		}

		return false;
	}

	bool PlayerJoinLeaveEvent::on_leave() noexcept
	{
		auto server_event = EventDispatcher::get_singleton_ptr()->find<NetworkServerEvent>("NetworkServerEvent");

		if (!server_event) return false;

		auto server = InstanceManager::get_singleton_ptr()->find<NetworkServerInstance>("NetworkServerInstance");

		if (!server) return false;

		for (size_t i = 0; i < server->size(); i++)
		{
			if (server->get(i).packet.CMD == NETWORK_CMD_STOP)
			{
				auto actors = InstanceManager::get_singleton_ptr()->find_all<ActorInstance>("ActorInstance");

				for (size_t y = 0; y < actors.size(); y++)
				{
					if (actors[y]->sockaddr().sin_addr.S_un.S_addr == server->get(i).addr.sin_addr.S_un.S_addr)
					{
						actors[y]->reset();

						--m_id_counter;

						return true;
					}
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}