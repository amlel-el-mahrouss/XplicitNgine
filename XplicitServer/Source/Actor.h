/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Actors are server-sided entities, they're used to
 *			describe Players and NPCs..
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class XPLICIT_API Actor final : public Instance
	{
	public:
		/*
		*	Replicated position structure
		*/

		class XPLICIT_API ActorPosition final
		{
		public:
			ActorPosition(float x, float y, float z)
				: X(x), Y(y), Z(z)
			{}

			~ActorPosition() {}

			ActorPosition& operator=(const ActorPosition&) = default;
			ActorPosition(const ActorPosition&) = default;

		public:
			float X;
			float Y;
			float Z;

		};

		// a delegate which tells who this actor belongs to.
		class XPLICIT_API ActorReplication final
		{
		public:
			ActorReplication()
				: addr(), cmd(), health(100), id(0)
			{}

			~ActorReplication() {}

			ActorReplication& operator=(const ActorReplication&) = default;
			ActorReplication(const ActorReplication&) = default;

		public:
			NETWORK_CMD cmd[XPLICIT_NETWORK_MAX_CMDS]; // Network Command Array.
			PrivateAddressData addr; // Actor's socket address
			int64_t health;
			int64_t id;

		};

	public:
		Actor();
		virtual ~Actor();

		Actor& operator=(const Actor&) = default;
		Actor(const Actor&) = default;

	public:
		void reset() noexcept; // reset the actor for future usage.

		void health(const int32_t& health) noexcept;
		const int32_t& health() noexcept; // gets the health of the actor.

		ActorReplication& get() noexcept; // gets the network replication data.
		ActorPosition& pos() noexcept; // gets its position

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;
		virtual bool should_update() noexcept override;

		void set(const float& x, const float& y, const float& z) noexcept;
		void set(const struct sockaddr_in& sockaddr) noexcept;

		virtual void update() override;

	public:
		// for the physics engine.
		virtual INSTANCE_PHYSICS physics() noexcept override;
		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;

	private:
		ActorReplication m_replication; /* Actor replication data */
		ActorPosition m_position; /* Actor's position */
		int32_t m_delay; /* Actor cooldown */

	};

	class XPLICIT_API ActorEvent final : public Event
	{
	public:
		ActorEvent() {}
		virtual ~ActorEvent() {}

		ActorEvent& operator=(const ActorEvent&) = default;
		ActorEvent(const ActorEvent&) = default;

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	};
}