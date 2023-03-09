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

#include "ServerInstance.h"
#include "MonoInstance.h"
#include "Actor.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const int32_t XPLICIT_ACTOR_COOLDOWN = 1000;

	/*
		Actor Instance methods.
	*/

	ActorInstance::ActorInstance(const bool human)
		: m_actor_health(100), m_replication(), m_pos(0, 0, 0), m_actor_delay(0), m_actor_id(-1)
	{
#ifndef _NDEBUG
		XPLICIT_INFO("ActorInstance::ActorInstance");
#endif
	}

	ActorInstance::~ActorInstance()
	{
#ifndef _NDEBUG
		XPLICIT_INFO("ActorInstance::~ActorInstance");
#endif
	}

	ActorInstance::INSTANCE_PHYSICS ActorInstance::physics() noexcept { return FAST; }

	bool ActorInstance::has_physics() noexcept { return true; }

	int64_t ActorInstance::id() noexcept { return m_actor_id; }

	void ActorInstance::update() 
	{
		auto server = Xplicit::InstanceManager::get_singleton_ptr()->find<Xplicit::NetworkServerInstance>("NetworkServerInstance");

		if (!server)
			return;

		if (this->get().cmd == NETWORK_CMD_DEAD)
		{
			if (this->m_actor_delay > 0)
			{
				--this->m_actor_delay;
			}
			else if (this->m_actor_delay < 1)
			{
				this->set(NETWORK_CMD_SPAWN);
				this->m_actor_delay = 0;

				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
					{
						server->get(i).packet.CMD = NETWORK_CMD_SPAWN;
						server->get(i).packet.ID = this->m_actor_id;

						break;
					}
				}
			}
		}

		// prepare a packet 

		if (this->health() < 1)
		{
			this->set(NETWORK_CMD_DEAD);
			this->m_actor_delay = XPLICIT_ACTOR_COOLDOWN;

			for (size_t i = 0; i < server->size(); i++)
			{
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
				{
					server->get(i).packet.CMD = NETWORK_CMD_DEAD;
					server->get(i).packet.ID = this->m_actor_id;

					break;
				}
			}
		}
		else if (this->get().cmd == NETWORK_CMD_POS)
		{
			// just do something as a simple as that
			// rely on the physics engine for collision detection.

			for (size_t i = 0; i < server->size(); i++)
			{
				if (server->get(i).addr.sin_addr.S_un.S_addr == this->m_replication.sockaddr.sin_addr.S_un.S_addr)
				{
					server->get(i).packet.CMD = NETWORK_CMD_POS;
					server->get(i).packet.ID = this->m_actor_id;

					server->get(i).packet.X = m_pos.X;
					server->get(i).packet.Y = m_pos.Y;
					server->get(i).packet.Z = m_pos.Y;

					break;
				}
			}
		}
	}

	int32_t& ActorInstance::health() noexcept { return m_actor_health; }
	bool ActorInstance::can_collide() noexcept { return true; }

	void ActorInstance::set(int64_t id) noexcept
	{
		if (id < 0) return;
		m_actor_id = id;
	}

	const char* ActorInstance::name() noexcept { return "ActorInstance"; }
	ActorInstance::INSTANCE_TYPE ActorInstance::type() noexcept { return ACTOR; }

	// Actor's Getters

	ActorInstance::ActorPosition& ActorInstance::pos() noexcept { return m_pos; }
	ActorInstance::ActorReplicationDelegate& ActorInstance::get() noexcept { return m_replication; }

	// Actor's Setters

	void ActorInstance::set(NETWORK_CMD cmd) noexcept { m_replication.cmd = cmd; }
	void ActorInstance::set(struct sockaddr_in sockaddr) noexcept { m_replication.sockaddr = sockaddr; }

	void ActorInstance::set(float x, float y, float z)  noexcept
	{
		m_pos.X = x;
		m_pos.X = y;
		m_pos.X = z;
	}

	void ActorInstance::reset() noexcept
	{
		m_replication.sockaddr.sin_addr.S_un.S_addr = 0;
		m_actor_health = 0;
		m_actor_id = -1;

		this->set(0.f, 0.f, 0.f);
	}

	const char* PhysicsActorEvent::name() noexcept { return ("PhysicsActorEvent"); }

	void PhysicsActorEvent::operator()()
	{
		auto vec_actor = InstanceManager::get_singleton_ptr()->find_all<ActorInstance>("ActorInstance");
		
		if (vec_actor.empty()) 
			return;

		// TODO: physics engine
	}
}