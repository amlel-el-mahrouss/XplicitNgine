/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Camera.cpp
 *			Purpose: Client-side camera
 *
 * =====================================================================
 */

#include "Application.h"
#include "Camera.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	CameraInstance::CameraInstance()
		: Instance(), m_camera(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeMaya(nullptr);
		XPLICIT_ASSERT(m_camera);

		m_camera->setName(this->name());
	}

	CameraInstance::~CameraInstance()
	{
		if (m_camera)
			m_camera->drop();

	}

	CameraInstance::INSTANCE_TYPE CameraInstance::type() noexcept { return INSTANCE_CAMERA; }
	const char* CameraInstance::name() noexcept { return ("CameraInstance"); }

	void CameraInstance::update()
	{
		
	}

	irr::scene::ICameraSceneNode* CameraInstance::get() noexcept { return m_camera; }
}