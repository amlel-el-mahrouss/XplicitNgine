/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Actors, or players
 *
 * =====================================================================
 */

#include "MonoInstance.h"
#include "Actor.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const int32_t XPLICIT_ACTOR_COOLDOWN = 1000;

	Actor::Actor(const bool human)
		: m_replication(), m_position(0, 0, 0), m_delay(0), m_actor_id(-1)
	{
	}

	Actor::~Actor()
	{
	}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_FAST; }

	bool Actor::has_physics() noexcept { return true; }

	const int64_t& Actor::id() noexcept { return m_actor_id; }

	void Actor::update() 
	{
		auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
		assert(server);

		if (this->get().cmd == NETWORK_CMD_DAMAGE)
		{
			for (size_t i = 0; i < server->size(); i++)
			{
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.addr.sin_addr.S_un.S_addr)
				{
					server->get(i).packet.CMD = NETWORK_CMD_DAMAGE;
					server->get(i).packet.ID = this->m_actor_id;
					server->get(i).packet.Health = this->health();

					server->send();

					return;
				}
			}
		}

		if (this->get().cmd == NETWORK_CMD_DEAD)
		{
			if (this->m_delay < 0)
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.addr.sin_addr.S_un.S_addr)
					{
						this->set(NETWORK_CMD_SPAWN);
						this->m_delay = 0;

						server->get(i).packet.CMD = NETWORK_CMD_SPAWN;
						server->get(i).packet.ID = this->m_actor_id;

						server->send();

						return;
					}
				}
			}
		}

		if (this->health() < 1)
		{
			for (size_t i = 0; i < server->size(); i++)
			{
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.addr.sin_addr.S_un.S_addr)
				{
					this->set(NETWORK_CMD_DEAD);
					this->m_delay = XPLICIT_ACTOR_COOLDOWN;

					server->get(i).packet.CMD = NETWORK_CMD_DEAD;
					server->get(i).packet.ID = this->m_actor_id;

					server->send();

					return;
				}
			}
		}
		
		for (size_t i = 0; i < server->size(); i++)
		{
			if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.addr.sin_addr.S_un.S_addr)
			{
				server->get(i).packet.CMD = NETWORK_CMD_POS;
				server->get(i).packet.ID = this->m_actor_id;

				switch (this->get().cmd)
				{
				case NETWORK_CMD_FORWARD:
				{
					server->get(i).packet.Z = 10.f;
					break;
				}
				case NETWORK_CMD_BACKWARDS:
				{
					server->get(i).packet.Z = -10.f;
					break;
				}
				case NETWORK_CMD_LEFT:
				{
					server->get(i).packet.X = -7.f;
					break;
				}
				case NETWORK_CMD_RIGHT:
				{
					server->get(i).packet.X = 7.f;
					break;
				}
				}

				server->send();

				return;
			}
		}
	}

	void Actor::health(const int32_t& health) noexcept 
	{ 
		this->get().health = health;
	}

	const int32_t& Actor::health() noexcept { return this->get().health; }

	bool Actor::can_collide() noexcept { return true; }

	const char* Actor::name() noexcept { return "Actor"; }
	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }

	// Actor's Getters

	Actor::ActorPosition& Actor::pos() noexcept { return m_position; }
	Actor::ActorReplication& Actor::get() noexcept { return m_replication; }

	// Actor's Setters

	void Actor::set(const int64_t& id) noexcept
	{
		if (id < 0) return;
		m_actor_id = id;
	}

	void Actor::set(const NETWORK_CMD& cmd) noexcept { m_replication.cmd = cmd; }
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

		m_actor_id = -1;
	}

	const char* ActorEvent::name() noexcept { return ("ActorEvent"); }

	void ActorEvent::operator()()
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");
		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		if (!server)
			return;

		if (actors.empty())
			return;

		for (size_t i = 0; i < actors.size(); i++)
		{
			if (!actors[i] || !actors[i]->can_collide() || !actors[i]->has_physics() || actors[i]->id() < 0)
				continue;

			for (size_t y = 0; y < server->size(); y++)
			{
				if (equals(server->get(y).addr, actors[i]->get().addr))
				{
					if (actors[i]->get().cmd == NETWORK_CMD_DEAD)
						continue;

					actors[i]->set(server->get(y).packet.CMD);
					actors[i]->set(server->get(y).packet.X, server->get(y).packet.Y, server->get(y).packet.Z);
				}
			}
		}
	}
}