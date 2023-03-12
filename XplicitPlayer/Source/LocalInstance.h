/*
 * =====================================================================
 *
 *			XplicitNgin C++ Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalInstance.h
 *			Purpose: Camera Controller
 *
 * =====================================================================
 */

#pragma once

#include "Instance.h"
#include "ApplicationContext.h"
#include "NetworkInstance.h"

namespace Xplicit::Client
{
	class LocalInstance : public Instance
	{
	public:
		LocalInstance();
		virtual ~LocalInstance();

		LocalInstance& operator=(const LocalInstance&) = default;
		LocalInstance(const LocalInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* m_camera;
		NetworkInstance* m_network;

	private:
		irr::video::ITexture* m_moving_tex;
		irr::video::ITexture* m_typing_tex;
		irr::video::ITexture* m_idle_tex;

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
