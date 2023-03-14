/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Instance.h
 *			Purpose: Xplicit's
 *
 * =====================================================================
 */

#include "Instance.h"

namespace Xplicit {
	void InstanceManager::update() noexcept
	{
		for (size_t i = 0; i < m_instances.size(); i++)
		{
			assert(m_instances[i]);

			// either delete or invalidate the instance.
			if (m_instances[i]->should_update())
				m_instances[i]->update();
		}
	}

	InstanceManager* InstanceManager::get_singleton_ptr()
	{
		static InstanceManager* ptr;

		if (!ptr)
			ptr = new InstanceManager();

		return ptr;
	}

	std::vector<Instance*>& InstanceManager::get() noexcept { return m_instances; }

	const char* Instance::name() noexcept { return ("Instance"); }

	Instance::INSTANCE_TYPE Instance::type() noexcept { return INSTANCE_LOGIC; }

	void Instance::update() {}

	bool Instance::can_collide() noexcept { return false; }
	bool Instance::has_physics() noexcept { return false; }

	bool Instance::should_update() noexcept { return true; }

	Instance::PHYSICS_TYPE Instance::physics() noexcept { return PHYSICS_FAST; }
}
