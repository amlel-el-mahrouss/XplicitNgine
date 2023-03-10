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
#include "Application.h"
#include "NetworkInstance.h"

namespace Xplicit
{
	class XPLICIT_API CameraInstance : public Instance
	{
	public:
		CameraInstance();
		virtual ~CameraInstance();

		CameraInstance& operator=(const CameraInstance&) = default;
		CameraInstance(const CameraInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return CAMERA; }
		virtual const char* name() noexcept override { return ("CameraInstance"); }

		virtual void update() override;
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* m_camera;
		irr::video::ITexture* m_moving_tex;
		irr::video::ITexture* m_typing_tex;
		irr::video::ITexture* m_idle_tex;

	};
}
