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

#pragma once

#include "Foundation.h"

namespace Xplicit 
{
	class Instance;
	class InstanceManager;

	class XPLICIT_API InstanceManager final 
	{
	private:
		InstanceManager() {}

	public:
		~InstanceManager() {}

		InstanceManager& operator=(const InstanceManager&) = default;
		InstanceManager(const InstanceManager&) = default;

		template <typename T>
		std::vector<T*> get_all(const char* name);

		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		bool remove(const char* name);

		template <typename T>
		T* get(const char* name);

		void update() noexcept;

	public:
		static InstanceManager* get_singleton_ptr();


	private:
		std::vector<Instance*> m_instances;

		friend class Instance;

	};

	class XPLICIT_API Instance 
	{
	public:
		Instance() {}
		virtual ~Instance() {}

		Instance& operator=(const Instance&) = default;
		Instance(const Instance&) = default;

		enum INSTANCE_TYPE : uint8_t 
		{
			MESH,
			ACTOR,
			LOGIC,
			CAMERA,
			SCRIPT,
			NETWORK,
			TERRAIN,
			INSTANCE, // classic instance, unchanged by the child class.
			INSTANCE_TYPE_COUNT
		};

		virtual const char* name() noexcept;
		virtual INSTANCE_TYPE type() noexcept;
		virtual void update();

		virtual bool can_collide() noexcept;
		virtual bool has_physics() noexcept;
		virtual bool should_update() noexcept;

		enum INSTANCE_PHYSICS : uint8_t
		{
			FAST,
			COMPLEX,
			INSTANCE_PHYSICS_COUNT,
		};

		virtual INSTANCE_PHYSICS physics() noexcept;

	};
}

#include "Instance.inl"