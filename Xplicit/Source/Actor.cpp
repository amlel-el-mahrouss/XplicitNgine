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
#ifndef _NDEBUG
		XPLICIT_INFO("Actor::Actor");
#endif
	}

	Actor::~Actor()
	{
#ifndef _NDEBUG
		XPLICIT_INFO("Actor::~Actor");
#endif
	}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_FAST; }

	bool Actor::has_physics() noexcept { return true; }

	const int64_t& Actor::id() noexcept { return m_actor_id; }

	void Actor::update() 
	{
		auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
		if (!server) return;

		if (this->get().cmd == NETWORK_CMD_DEAD)
		{
			if (this->m_delay < 0)
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
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
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
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
		
		if (this->get().cmd == NETWORK_CMD_POS)
		{
			// just do something as a simple as that
			// rely on the physics engine for collision detection.

			for (size_t i = 0; i < server->size(); i++)
			{
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
				{
					server->get(i).packet.CMD = NETWORK_CMD_POS;
					server->get(i).packet.ID = this->m_actor_id;

					server->get(i).packet.X = m_position.X;
					server->get(i).packet.Y = m_position.Y;
					server->get(i).packet.Z = m_position.Y;

					server->send();

					return;
				}
			}
		}
	}

	void Actor::health(const int32_t& health) noexcept 
	{ 
		this->get().health = health;
	}

	const int32_t& Actor::health() noexcept { return this->get().health; }

	bool Actor::can_collide() noexcept { return true; }

	void Actor::set(int64_t id) noexcept
	{
		if (id < 0) return;
		m_actor_id = id;
	}

	const char* Actor::name() noexcept { return "Actor"; }
	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }

	// Actor's Getters

	Actor::ActorPosition& Actor::pos() noexcept { return m_position; }
	Actor::ActorReplication& Actor::get() noexcept { return m_replication; }

	// Actor's Setters

	void Actor::set(NETWORK_CMD cmd) noexcept { m_replication.cmd = cmd; }
	void Actor::set(struct sockaddr_in sockaddr) noexcept { m_replication.sockaddr = sockaddr; }

	void Actor::set(float x, float y, float z)  noexcept
	{
		m_position.X = x;
		m_position.X = y;
		m_position.X = z;
	}

	void Actor::reset() noexcept
	{
		this->get().sockaddr.sin_addr.S_un.S_addr = 0;
		this->set(0.f, 0.f, 0.f);
		this->get().health = 0;

		m_actor_id = -1;
	}
}