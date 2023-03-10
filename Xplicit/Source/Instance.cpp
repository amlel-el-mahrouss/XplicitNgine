/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Instance.h
 *			Purpose: Xplicit's ECS
 *
 * =====================================================================
 */

#include "Instance.h"

namespace Xplicit {
	void InstanceManager::update() noexcept
	{
		for (size_t i = 0; i < m_instances.size(); i++)
		{
			// either delete or invalidate the instance.
			if (m_instances[i])
			{
				if (m_instances[i]->should_update())
					m_instances[i]->update();
			}
		}
	}

	InstanceManager* InstanceManager::get_singleton_ptr()
	{
		static InstanceManager* ptr;
		if (!ptr)
			ptr = new InstanceManager();

		return ptr;
	}

	const char* Instance::name() noexcept { return ("Instance"); }
	Instance::INSTANCE_TYPE Instance::type() noexcept { return INSTANCE; }
	void Instance::update() {}

	bool Instance::can_collide() noexcept { return false; }
	bool Instance::has_physics() noexcept { return false; }

	bool Instance::should_update() noexcept { return true; }

	Instance::INSTANCE_PHYSICS Instance::physics() noexcept { return FAST; }
}
