/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
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
#include "XplicitApp.h"
#include "Instance.h"
#include "Event.h"

namespace Xplicit
{
	// Server-Side Actor Class
	class XPLICIT_API ActorInstance final : public Instance
	{
	public:
		// an actor position structure.
		// used to describe where it is.

		class ActorPos final
		{
		public:
			ActorPos(float x, float y, float z)
				: X(x), Y(y), Z(z)
			{}

			~ActorPos() {}

			ActorPos& operator=(const ActorPos&) = default;
			ActorPos(const ActorPos&) = default;

		public:
			float X;
			float Y;
			float Z;

		};

		class ActorReplication final
		{
		public:
			ActorReplication()
				: sockaddr(), cmd(Xplicit::NETWORK_CMD_INVALID)
			{}

			~ActorReplication() {}

			ActorReplication& operator=(const ActorReplication&) = default;
			ActorReplication(const ActorReplication&) = default;

		public:
			struct sockaddr_in sockaddr; // Actor's socket address
			NETWORK_CMD cmd; // Actor's current network command.

		};

	public:
		explicit ActorInstance(const bool human = true);
		virtual ~ActorInstance();

		ActorInstance& operator=(const ActorInstance&) = default;
		ActorInstance(const ActorInstance&) = default;

	public:
		int16_t& health() noexcept; // gets the health of the actor.
		void reset() noexcept; // reset and reserve actor for future usage
		int64_t id() noexcept; // gets it's id, assigned by a system.

		ActorReplication& get() noexcept; // gets the network replication data.
		ActorPos& pos() noexcept; // gets its position

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

		void set(int64_t id) noexcept;
		void set(NETWORK_CMD cmd) noexcept;
		void set(float x, float y, float z) noexcept;
		void set(struct sockaddr_in sockaddr) noexcept;

		virtual void update() override;

	public:
		virtual INSTANCE_PHYSICS physics() noexcept override;

		virtual bool can_collide() override;
		virtual bool has_physics() override;
		bool is_colliding() noexcept;

	private:
		ActorReplication m_replication;

	private:
		ActorPos m_pos;

	private:
		int32_t m_respawn_delay;
		int16_t m_actor_health;
		int64_t m_actor_id;

		friend class PlayerActorEvent;

	};

	class XPLICIT_API NetworkActorEvent final : public Event
	{
	public:
		NetworkActorEvent() {}
		virtual ~NetworkActorEvent() {}

		NetworkActorEvent& operator=(const NetworkActorEvent&) = default;
		NetworkActorEvent(const NetworkActorEvent&) = default;

		virtual const char* name() noexcept { return ("NetworkActorEvent"); }
		virtual void operator()();

	};

	class XPLICIT_API PlayerActorEvent final : public Event
	{
	public:
		PlayerActorEvent() {}
		virtual ~PlayerActorEvent() {}

		PlayerActorEvent& operator=(const PlayerActorEvent&) = default;
		PlayerActorEvent(const PlayerActorEvent&) = default;

		virtual const char* name() noexcept { return ("PlayerActorEvent"); }

		virtual void operator()();

	};

	class XPLICIT_API PhysicsActorEvent final : public Event
	{
	public:
		PhysicsActorEvent() {}
		virtual ~PhysicsActorEvent() {}

		PhysicsActorEvent& operator=(const PhysicsActorEvent&) = default;
		PhysicsActorEvent(const PhysicsActorEvent&) = default;

		virtual const char* name() noexcept { return ("ActorPhysicsEvent"); }

		virtual void operator()();

	};
}