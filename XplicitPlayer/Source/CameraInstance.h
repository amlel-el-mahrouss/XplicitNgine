/*
 * =====================================================================
 *
 *			XplicitNgin C++ Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: CameraInstance.h
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
	class CameraInstance : public Instance
	{
	public:
		CameraInstance();
		virtual ~CameraInstance();

		CameraInstance& operator=(const CameraInstance&) = default;
		CameraInstance(const CameraInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* m_camera;

	private:
		irr::video::ITexture* m_moving_tex;
		irr::video::ITexture* m_typing_tex;
		irr::video::ITexture* m_idle_tex;

	};
}
