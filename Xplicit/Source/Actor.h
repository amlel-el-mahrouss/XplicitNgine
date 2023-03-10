/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Actor.h
 *			Purpose: Actors are server-sided entities, they're used to
 *			describe Players and NPCs..
 *
 * =====================================================================
 */

#pragma once

#include "ServerInstance.h"
#include "Foundation.h"
#include "Instance.h"
#include "Event.h"

namespace Xplicit
{
	class XPLICIT_API Actor final : public Instance
	{
	public:
		// an actor position structure.
		// used to describe where it is.
		class ActorPosition final
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
		class ActorReplication final
		{
		public:
			ActorReplication()
				: sockaddr(), cmd(Xplicit::NETWORK_CMD_INVALID), health(100)
			{}

			~ActorReplication() {}

			ActorReplication& operator=(const ActorReplication&) = default;
			ActorReplication(const ActorReplication&) = default;

		public:
			struct sockaddr_in sockaddr; // Actor's socket address
			NETWORK_CMD cmd; // Actor's current network command.
			int64_t health;

		};

	public:
		explicit Actor(const bool human = true);
		virtual ~Actor();

		Actor& operator=(const Actor&) = default;
		Actor(const Actor&) = default;

	public:
		void reset() noexcept; // reset the actor for future usage.

		void health(const int32_t& health) noexcept;
		const int32_t& health() noexcept; // gets the health of the actor.
		const int64_t& id() noexcept; // gets it's id, assigned by a system.

		ActorReplication& get() noexcept; // gets the network replication data.
		ActorPosition& pos() noexcept; // gets its position

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		// useful setters.
		void set(struct sockaddr_in sockaddr) noexcept;
		void set(float x, float y, float z) noexcept;
		void set(NETWORK_CMD cmd) noexcept;
		void set(int64_t id) noexcept;

		virtual void update() override;

	public:
		// for the physics engine.
		virtual INSTANCE_PHYSICS physics() noexcept override;
		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;

	private:
		ActorReplication m_replication;
		ActorPosition m_pos;
		int64_t m_actor_id;
		int32_t m_delay;

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