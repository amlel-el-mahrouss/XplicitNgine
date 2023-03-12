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

	Actor::Actor(const bool human)
		: m_replication(), m_position(0, 0, 0), m_delay(0)
	{

	}

	Actor::~Actor()
	{
	}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_FAST; }

	bool Actor::has_physics() noexcept { return true; }

	void Actor::update() 
	{
		auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			throw EngineError();

// little helper
// CMD = network command, COORD = either X, Y or Z.
#define XPLICIT_SET_POS_CMD(_CMD, COORD, VEL, IDX)\
				server->get(i).packet.CMD[IDX] = _CMD;\
				server->get(i).packet.COORD = this->pos().COORD = VEL;\
				server->get(i).packet.ID = this->get().id;



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

	bool Actor::can_collide() noexcept { return true; }

	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }
	const char* Actor::name() noexcept { return "Actor"; }

	bool Actor::should_update() noexcept { return true; }

	Actor::ActorPosition& Actor::pos() noexcept { return m_position; }
	Actor::ActorReplication& Actor::get() noexcept { return m_replication; }

	void Actor::set(const struct sockaddr_in& sockaddr) noexcept { m_replication.addr = sockaddr; }

	void Actor::set(const float& x, const float& y, const float& z)  noexcept
	{
		m_position.X = x;
		m_position.X = y;
		m_position.X = z;
	}

	void Actor::reset() noexcept
	{
		memset(&this->get().addr, 0, sizeof(struct sockaddr_in));

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

			for (size_t y = 0; y < server->size(); y++)
			{
				if (equals(server->get(y).addr, actors[i]->get().addr))
				{
					memcpy(actors[i]->get().cmd, server->get(y).packet.CMD, XPLICIT_NETWORK_MAX_CMDS);
					actors[i]->set(server->get(y).packet.X, server->get(y).packet.Y, server->get(y).packet.Z);
				}
			}
		}
	}
}