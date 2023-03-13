/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Camera.h
 *			Purpose: Client-side misc instances
 *
 * =====================================================================
 */

#pragma once

#include "Instance.h"
#include "ApplicationContext.h"
#include "NetworkInstance.h"

namespace Xplicit::Client
{
	// The 'player' in-person.
	class LocalCameraInstance : public Instance
	{
	public:
		LocalCameraInstance();
		virtual ~LocalCameraInstance();

		LocalCameraInstance& operator=(const LocalCameraInstance&) = default;
		LocalCameraInstance(const LocalCameraInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;

	public:
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* m_camera;

	private:
		irr::video::ITexture* m_moving_tex;
		irr::video::ITexture* m_typing_tex;
		irr::video::ITexture* m_idle_tex;

	private:
		NetworkInstance* m_network;

	};

	class LocalWatchdogEvent : public Event
	{
	public:
		LocalWatchdogEvent();
		~LocalWatchdogEvent();

		LocalWatchdogEvent& operator=(const LocalWatchdogEvent&) = default;
		LocalWatchdogEvent(const LocalWatchdogEvent&) = default;

		virtual const char* name() noexcept override { return ("LocalWatchdogEvent"); }
		virtual void operator()() override;

	private:
		NetworkInstance* m_network;

	};
}
