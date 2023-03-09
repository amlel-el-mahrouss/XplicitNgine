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
#include "XplicitApp.h"
#include "Instance.h"
#include "Event.h"

namespace Xplicit
{
	class XPLICIT_API ActorInstance final : public Instance
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
		class ActorReplicationDelegate final
		{
		public:
			ActorReplicationDelegate()
				: sockaddr(), cmd(Xplicit::NETWORK_CMD_INVALID)
			{}

			~ActorReplicationDelegate() {}

			ActorReplicationDelegate& operator=(const ActorReplicationDelegate&) = default;
			ActorReplicationDelegate(const ActorReplicationDelegate&) = default;

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
		int32_t& health() noexcept; // gets the health of the actor.
		void reset() noexcept; // reset and reserve actor for future usage
		int64_t id() noexcept; // gets it's id, assigned by a system.

		ActorReplicationDelegate& get() noexcept; // gets the network replication data.
		ActorPosition& pos() noexcept; // gets its position

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

		void set(int64_t id) noexcept;
		void set(NETWORK_CMD cmd) noexcept;
		void set(float x, float y, float z) noexcept;
		void set(struct sockaddr_in sockaddr) noexcept;

		virtual void update() override;

	public:
		virtual INSTANCE_PHYSICS physics() noexcept override;

		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;

	private:
		ActorReplicationDelegate m_replication;
		ActorPosition m_pos;

	private:
		int32_t m_actor_health;
		int32_t m_actor_delay;
		int64_t m_actor_id;

		friend class PlayerActorEvent;

	};

	class XPLICIT_API PhysicsActorEvent final : public Event
	{
	public:
		PhysicsActorEvent() {}
		virtual ~PhysicsActorEvent() {}

		PhysicsActorEvent& operator=(const PhysicsActorEvent&) = default;
		PhysicsActorEvent(const PhysicsActorEvent&) = default;

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	};
}