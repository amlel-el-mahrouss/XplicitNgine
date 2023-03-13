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

	Actor::Actor() : m_peer(nullptr) {}

	Actor::~Actor() {}

	Actor::INSTANCE_PHYSICS Actor::physics() noexcept { return PHYSICS_NONE; }

	bool Actor::has_physics() noexcept { return false; }

	void Actor::update()
	{
		if (!m_peer)
			return;

		auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
		XPLICIT_ASSERT(server);

		// little helper
		// CMD = network command, COORD = either X, Y or Z.
#define XPLICIT_SET_POS_CMD(COORD, VEL)\
				m_peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;\
				m_peer->packet.COORD = VEL;\
				m_peer->packet.id = this->m_peer->uuid_hash;


		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
		{
			m_peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			XPLICIT_SET_POS_CMD(Z, 100.f);
		}

		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARDS)
		{
			m_peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
			XPLICIT_SET_POS_CMD(Z, -100.f);
		}

		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
		{
			m_peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
			XPLICIT_SET_POS_CMD(X, -100.f);
		}

		if (m_peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
		{
			m_peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
			XPLICIT_SET_POS_CMD(X, 100.f);
		}

#undef XPLICIT_SET_POS_CMD

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