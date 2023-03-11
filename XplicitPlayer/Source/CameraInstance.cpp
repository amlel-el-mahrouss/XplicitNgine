/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: CameraInstance.cpp
 *			Purpose: Client Camera
 *
 * =====================================================================
 */

#include "CameraInstance.h"

namespace Xplicit::Client
{
	CameraInstance::CameraInstance()
		: Instance(), m_camera(nullptr), m_idle_tex(nullptr), m_moving_tex(nullptr), m_typing_tex(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeFPS(nullptr, 60.F, 0.2F);
		IRR->getCursorControl()->setVisible(false);

		m_camera->setName("CameraInstance");

		XPLICIT_GET_DATA_DIR(data_dir);

		if (!data_dir)
			throw EngineError();

		std::string idle_path = data_dir;
		idle_path += "\\Textures\\idle.png";

		m_idle_tex = IRR->getVideoDriver()->getTexture(idle_path.c_str());

		std::string moving_path = data_dir;
		moving_path += "\\Textures\\moving.png";

		m_moving_tex = IRR->getVideoDriver()->getTexture(moving_path.c_str());

		std::string typing_path = data_dir;
		typing_path += "\\Textures\\typing.png";

		m_typing_tex = IRR->getVideoDriver()->getTexture(typing_path.c_str());
	}

	CameraInstance::~CameraInstance()
	{
		if (m_camera)
			m_camera->drop();

		if (m_moving_tex)
			m_moving_tex->drop();

		if (m_typing_tex)
			m_typing_tex->drop();

		if (m_idle_tex)
			m_idle_tex->drop();
	}


	CameraInstance::INSTANCE_TYPE CameraInstance::type() noexcept { return INSTANCE_CAMERA; }
	const char* CameraInstance::name() noexcept { return ("CameraInstance"); }

	void CameraInstance::update()
	{
		auto pos = KB->get_pos();

		if (KB->key_down(irr::KEY_KEY_W))
		{
			IRR->getVideoDriver()->draw2DImage(m_moving_tex, vector2di(pos.X, pos.Y),
				core::rect<s32>(0, 0, 38, 38), 0,
				video::SColor(255, 255, 255, 255), true);
		}
		else if (KB->key_down(irr::KEY_KEY_T))
		{
			IRR->getVideoDriver()->draw2DImage(m_typing_tex, vector2di(pos.X, pos.Y),
				core::rect<s32>(0, 0, 38, 38), 0,
				video::SColor(255, 255, 255, 255), true);
		}
		else
		{
			IRR->getVideoDriver()->draw2DImage(m_idle_tex, vector2di(pos.X, pos.Y),
				core::rect<s32>(0, 0, 38, 38), 0,
				video::SColor(255, 255, 255, 255), true);
		}
	}

	irr::scene::ICameraSceneNode* CameraInstance::get() noexcept { return m_camera; }
}