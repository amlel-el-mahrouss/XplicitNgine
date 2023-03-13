/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.cpp
 *			Purpose: Actor Logic.
 *
 * =====================================================================
 */

#include "Actor.h"

namespace Xplicit
{
	constexpr const int32_t XPLICIT_ACTOR_COOLDOWN = 100;

	Actor::Actor() : m_peer(nullptr), m_health(0) {}

	Actor::~Actor() {}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_NONE; }

	bool Actor::has_physics() noexcept { return false; }

	void Actor::update()
	{
		if (!m_peer)
			return;

		if (this->health() < 0)
		{
			m_peer->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
		}

		m_peer->packet.health = this->health();
		m_peer->packet.id = m_peer->uuid_hash;

		// TODO: 
	}

	void Actor::health(const int32_t& health) noexcept { this->m_health = health; }
	const int32_t& Actor::health() noexcept { return this->m_health; }

	bool Actor::can_collide() noexcept { return false; }

	Actor::INSTANCE_TYPE Actor::type() noexcept { return INSTANCE_ACTOR; }
	const char* Actor::name() noexcept { return "Actor"; }

	bool Actor::should_update() noexcept { return true; }

	NetworkPeer* Actor::get() noexcept { return m_peer; }

	void Actor::set(NetworkPeer* peer) noexcept 
	{
		XPLICIT_ASSERT(peer);
		m_peer = peer;
	}
}