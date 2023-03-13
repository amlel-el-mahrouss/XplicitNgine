/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.cpp
 *			Purpose: Actors, or players
 *
 * =====================================================================
 */

#include "Actor.h"

namespace Xplicit
{
	constexpr const int32_t XPLICIT_ACTOR_COOLDOWN = 100;

	Actor::Actor()
		: m_replicated(), m_position(0, 0, 0)
	{

	}

	Actor::~Actor()
	{

	}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_NONE; }

	bool Actor::has_physics() noexcept { return false; }

	void Actor::update()
	{
		auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
		XPLICIT_ASSERT(server);

		// little helper
		// CMD = network command, COORD = either X, Y or Z.
#define XPLICIT_SET_POS_CMD(_CMD, COORD, VEL, IDX)\
				server->get(i).packet.cmd[IDX] = _CMD;\
				server->get(i).packet.COORD = this->pos().COORD = VEL;\
				server->get(i).packet.id = this->get().uuid_hash;



		for (size_t i = 0; i < server->size(); i++)
		{
			if (equals(server->get(i).addr, this->get().addr))
			{
				for (size_t z = 0; z < XPLICIT_NETWORK_MAX_CMDS; z++)
				{
					switch (this->get().cmd[z])
					{
					case NETWORK_CMD_FORWARD:
					{
						XPLICIT_SET_POS_CMD(NETWORK_CMD_FORWARD, Y, 10.f, XPLICIT_NETWORK_CMD_FORWARD);
						break;
					}
					case NETWORK_CMD_BACKWARDS:
					{
						XPLICIT_SET_POS_CMD(NETWORK_CMD_BACKWARDS, Y, -10.f, XPLICIT_NETWORK_CMD_BACKWARD);
						break;
					}
					case NETWORK_CMD_LEFT:
					{
						XPLICIT_SET_POS_CMD(NETWORK_CMD_LEFT, X, -10.f, XPLICIT_NETWORK_CMD_LEFT);
						break;
					}
					case NETWORK_CMD_RIGHT:
					{
						XPLICIT_SET_POS_CMD(NETWORK_CMD_RIGHT, X, 10.f, XPLICIT_NETWORK_CMD_RIGHT);
						break;
					}
					}

				}

				break;
			}
		}

		server->send();

#undef XPLICIT_SET_POS_CMD

	}

	void Actor::health(const int32_t& health) noexcept { this->get().health = health; }
	const int32_t& Actor::health() noexcept { return this->get().health; }

	bool Actor::can_collide() noexcept { return false; }

	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }
	const char* Actor::name() noexcept { return "Actor"; }

	bool Actor::should_update() noexcept { return true; }

	Actor::ActorPosition& Actor::pos() noexcept { return m_position; }
	Actor::ActorReplication& Actor::get() noexcept { return m_replicated; }

	void Actor::set(const float& x, const float& y, const float& z)  noexcept
	{
		m_position.X = x;
		m_position.X = y;
		m_position.X = z;
	}

	void Actor::reset() noexcept
	{
		memset(&this->m_replicated.addr, 0, sizeof(PrivateAddressData));
		memset(&this->m_replicated.cmd, 0, XPLICIT_NETWORK_MAX_CMDS);

		this->set(0.f, 0.f, 0.f);
		this->get().health = 0;
	}

	const char* ActorEvent::name() noexcept { return ("ActorEvent"); }

	void ActorEvent::operator()()
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");
		if (!server) return;

		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");
		if (actors.empty()) return;

		for (size_t i = 0; i < actors.size(); i++)
		{
			if (!actors[i])
				continue;

			for (size_t server_cl = 0; server_cl < server->size(); server_cl++)
			{
				if (equals(server->get(server_cl).addr, actors[i]->get().addr))
				{
					for (size_t cmd_index = 0; cmd_index < XPLICIT_NETWORK_MAX_CMDS; cmd_index++)
					{
						auto cmd = server->get(server_cl).packet.cmd[cmd_index];
						actors[i]->get().cmd[cmd_index] = cmd;
					}

					actors[i]->set(server->get(server_cl).packet.X, server->get(server_cl).packet.Y, server->get(server_cl).packet.Z);
				}
			}
		}
	}
}