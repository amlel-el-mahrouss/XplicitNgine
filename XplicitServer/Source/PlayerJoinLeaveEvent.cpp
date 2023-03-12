/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event
 *
 * =====================================================================
 */

#include "PlayerJoinLeaveEvent.h"
#include "Actor.h"

namespace Xplicit
{
	static void xplicit_join_event(NetworkPeer& cl, Actor* actor, NetworkServerInstance* server, int64_t& counter)
	{
		actor->get().id = counter;
		cl.id = counter;

		cl.packet.CMD[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
		cl.packet.ID = actor->get().id;
		cl.stat = NETWORK_STAT_CONNECTED;

		server->send();

		// force update
		auto env = EventDispatcher::get_singleton_ptr()->get<NetworkServerEvent>("NetworkServerEvent");
		XPLICIT_ASSERT(env);

		env->update();

		++counter;
	}

	static void xplicit_leave_event(NetworkPeer& cl, Actor* actor, NetworkServerInstance* server, int64_t& counter)
	{
		--counter;
		cl.reset();
		InstanceManager::get_singleton_ptr()->remove<Actor>(actor);
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() : m_id_counter(0) {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() {}

	void PlayerJoinLeaveEvent::operator()()
	{
		// the main logic for the join and leave event.
		// here we simply handle player logic.
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		this->on_join(server);
		this->on_leave(server);
	}

	bool PlayerJoinLeaveEvent::on_join(NetworkServerInstance* server) noexcept
	{
		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx).stat == NETWORK_STAT_CONNECTED)
				continue;

			if (server->get(peer_idx).packet.CMD[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN)
			{
				auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

				for (size_t actor_idx = 0; actor_idx < actors.size(); ++actor_idx)
				{
					if (actors[actor_idx]->get().id == -1)
					{
						xplicit_join_event(server->get(peer_idx), actors[actor_idx], server, m_id_counter);

						XPLICIT_INFO("[CONNECT] Unique Connection ID: " + uuids::to_string(server->get(peer_idx).unique_addr.uuid));
					}
				}

				auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
					throw EngineError();

				xplicit_join_event(server->get(peer_idx), actor, server, m_id_counter);

				XPLICIT_INFO("[CONNECT] Unique Connection ID: " + uuids::to_string(server->get(peer_idx).unique_addr.uuid));
			}
		}

		return false;
	}

	bool PlayerJoinLeaveEvent::on_leave(NetworkServerInstance* server) noexcept
	{
		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx).stat == NETWORK_STAT_DISCONNECTED)
				continue;

			if (server->get(peer_idx).packet.CMD[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				server->get(peer_idx).packet.CMD[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

				for (size_t actor_idx = 0; actor_idx < actors.size(); ++actor_idx)
				{
					if (actors[actor_idx]->get().id == server->get(peer_idx).id)
					{
						XPLICIT_INFO("[DISCONNECT] Unique Connection ID: " + uuids::to_string(server->get(peer_idx).unique_addr.uuid));
						
						xplicit_leave_event(server->get(peer_idx), actors[actor_idx], server, m_id_counter);
					}
				}
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}