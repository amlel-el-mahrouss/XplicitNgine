/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalPlayer.h
 *			Purpose: Client-side Player Instance
 *
 * =====================================================================
 */

#pragma once

#include "Instance.h"
#include "Application.h"
#include "CameraInstance.h"
#include "NetworkInstance.h"

// FIXME: rework them when we will roll our own renderer.
namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;
	constexpr const float XPLICIT_SPEED = 20.f;

	class LocalActor : public Instance
	{
	public:
		LocalActor(int64_t id);
		virtual ~LocalActor();

		LocalActor& operator=(const LocalActor&) = default;
		LocalActor(const LocalActor&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return CAMERA; }
		virtual const char* name() noexcept override { return ("LocalActor"); }

		virtual void update() override;

	private:
		NetworkInstance* m_net;
		CameraInstance* m_cam;
		int64_t m_id;
		u32 m_then;

	};

	class XPLICIT_API LocalActorMoveEvent : public Event
	{
	public:
		LocalActorMoveEvent();
		virtual ~LocalActorMoveEvent();

		LocalActorMoveEvent& operator=(const LocalActorMoveEvent&) = default;
		LocalActorMoveEvent(const LocalActorMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		irr::core::vector3df m_last_pos;
		NetworkPacket m_packet;

	};
}
