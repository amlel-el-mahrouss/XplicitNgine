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

namespace Xplicit
{
	static size_t xplicit_hash_from_uuid(uuids::uuid& uuid)
	{
		std::string uuid_str = uuids::to_string(uuid);
		auto hash = std::hash<std::string>();
		auto res = hash(uuid_str);
		
		return res;
	}

	static void xplicit_join_event(NetworkPeer& cl, Actor* actor, NetworkServerInstance* server)
	{
		actor->get().uuid = cl.unique_addr.uuid;

		auto hash = xplicit_hash_from_uuid(actor->get().uuid);
		actor->get().uuid_hash = hash;

		cl.packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

		cl.packet.id = hash;
		cl.stat = NETWORK_STAT_CONNECTED;

		server->send();
	}

	static bool xplicit_leave_event(NetworkPeer& cl, NetworkServerInstance* server)
	{
		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		for (size_t i = 0; i < actors.size(); i++)
		{
			if (actors[i]->get().uuid == cl.unique_addr.uuid)
			{
				cl.reset();

				return true;
			}
		}

		return false;
	}

	PlayerJoinLeaveEvent::PlayerJoinLeaveEvent() {}
	PlayerJoinLeaveEvent::~PlayerJoinLeaveEvent() {}

	void PlayerJoinLeaveEvent::operator()()
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		this->join_event(server);
		this->leave_event(server);
	}

	bool PlayerJoinLeaveEvent::join_event(NetworkServerInstance* server) noexcept
	{
		if (!server)
			return false;

		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx).stat == NETWORK_STAT_CONNECTED)
				continue;

			if (server->get(peer_idx).packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] == NETWORK_CMD_BEGIN)
			{
				auto actor = InstanceManager::get_singleton_ptr()->add<Actor>();

				if (!actor)
				{
					server->get(peer_idx).packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
					server->get(peer_idx).packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_INVALID;

					return false;
				}

				xplicit_join_event(server->get(peer_idx), actor, server);

				XPLICIT_INFO("[CONNECT] Unique Connection ID: " + uuids::to_string(server->get(peer_idx).unique_addr.uuid));
			}
		}

		return true;
	}

	bool PlayerJoinLeaveEvent::leave_event(NetworkServerInstance* server) noexcept
	{
		if (!server)
			return false;

		for (size_t peer_idx = 0; peer_idx < server->size(); ++peer_idx)
		{
			if (server->get(peer_idx).stat == NETWORK_STAT_DISCONNECTED)
				continue;

			if (server->get(peer_idx).packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP ||
				server->get(peer_idx).packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (xplicit_leave_event(server->get(peer_idx), server))
					XPLICIT_INFO("[DISCONNECT] Unique Connection ID: " + uuids::to_string(server->get(peer_idx).unique_addr.uuid));
			}
		}

		return false;
	}

	const char* PlayerJoinLeaveEvent::name() noexcept { return ("PlayerJoinLeaveEvent"); }
}