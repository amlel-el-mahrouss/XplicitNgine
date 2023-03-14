/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: RigidBody.h
 *			Purpose: Rigid Bodies simple implementation
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

#include <Instance.h>

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	struct RigidBody
	{
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;
		TypeFloat Mass;

	};

	template <typename TypeFloat = float>
	class RigidBodyInstance : public Instance
	{
	public:
		RigidBodyInstance() : Instance(), m_rigid_bodies(), m_gravity(0, -9.81f, 0) {}
		virtual ~RigidBodyInstance() = default;

		RigidBodyInstance& operator=(const RigidBodyInstance&) = delete;
		RigidBodyInstance(const RigidBodyInstance&) = delete;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_PHYSICS; }
		virtual const char* name() noexcept override { return ("RigidBodyInstance"); }

		virtual void update() override
		{
			for (RigidBody<TypeFloat>* rigid : m_rigid_bodies)
			{
				rigid->Force.mul(m_gravity.X * rigid->Mass, m_gravity.Y * rigid->Mass, m_gravity.Z * rigid->Mass);

				rigid->Velocity.add(rigid->Force.X / rigid->Mass * m_delta, 
					rigid->Force.Y / rigid->Mass * m_delta, 
					rigid->Force.Z / rigid->Mass * m_delta);

				rigid->Position.add(rigid->Velocity.X * m_delta, rigid->Velocity.Y * m_delta, rigid->Velocity.Z * m_delta);

				rigid->Force = Vector<TypeFloat>(0, 0, 0);

				++m_delta;
			}
		}

	public:
		void set(const Vector<float>& gravity) noexcept { m_gravity.add(gravity.X, gravity.Y, gravity.Z); }
		Vector<float>& get() noexcept { return m_gravity; }

		void add(RigidBody<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			m_rigid_bodies.push_back(rigid);
		}

		void remove(RigidBody<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			auto it = std::find(m_rigid_bodies.cbegin(), m_rigid_bodies.cend(), rigid);

			if (it != m_rigid_bodies.cend())
				m_rigid_bodies.erase(it);
		}

	private:
		std::vector<RigidBody<TypeFloat>*> m_rigid_bodies;
		Vector<float> m_gravity;
		int64_t m_delta;

	};

	using RigidBodyInstanceF32 = RigidBodyInstance<float>;
}