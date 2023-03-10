/*
 * =====================================================================
 *
 *			XplicitNgin C++ Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalPLayerInstance.h
 *			Purpose: Client-side Player Instance
 *
 * =====================================================================
 */

#pragma once

#include "Instance.h"
#include "Application.h"
#include "CameraInstance.h"
#include "NetworkInstance.h"


namespace Xplicit
{
	constexpr const int XPLICIT_NETWORK_DELAY = 1000;
	constexpr const float XPLICIT_SPEED = 20.f;

	class XPLICIT_API LocalPlayerInstance : public Instance
	{
	public:
		LocalPlayerInstance(int64_t id);
		virtual ~LocalPlayerInstance();

		LocalPlayerInstance& operator=(const LocalPlayerInstance&) = default;
		LocalPlayerInstance(const LocalPlayerInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return CAMERA; }
		virtual const char* name() noexcept override { return ("LocalPlayerInstance"); }

		virtual void update() override;

	private:
		NetworkInstance* m_net;
		CameraInstance* m_cam;
		int64_t m_id;
		u32 m_then;

	};

	class XPLICIT_API LocalPlayerMoveEvent : public Event
	{
	public:
		LocalPlayerMoveEvent();
		virtual ~LocalPlayerMoveEvent();

		LocalPlayerMoveEvent& operator=(const LocalPlayerMoveEvent&) = default;
		LocalPlayerMoveEvent(const LocalPlayerMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		irr::core::vector3df m_last_pos;
		int64_t m_cooldown;

	};
}
