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
#include "XUI.h"

namespace Xplicit::Client
{
	LocalCameraInstance::LocalCameraInstance()
		: Instance(), m_camera(nullptr), m_idle_tex(nullptr), 
		m_moving_tex(nullptr), m_typing_tex(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeFPS(nullptr, 100.f, 0.f);
		XPLICIT_ASSERT(m_camera);

		m_camera->setPosition(vector3df(0, 0, 0));

		IRR->getCursorControl()->setVisible(false);

		m_camera->setName("LocalCameraInstance");

		XPLICIT_GET_DATA_DIR(data_dir);

		if (!data_dir)
			throw EngineError();

		// precache textures.

		std::string idle_path = data_dir;
		idle_path += "\\Textures\\idle.png";

		m_idle_tex = IRR->getVideoDriver()->getTexture(idle_path.c_str());

		std::string moving_path = data_dir;
		moving_path += "\\Textures\\moving.png";

		m_moving_tex = IRR->getVideoDriver()->getTexture(moving_path.c_str());

		std::string typing_path = data_dir;
		typing_path += "\\Textures\\typing.png";

		m_typing_tex = IRR->getVideoDriver()->getTexture(typing_path.c_str());

		XPLICIT_ASSERT(m_typing_tex);
		XPLICIT_ASSERT(m_moving_tex);
		XPLICIT_ASSERT(m_idle_tex);
	}

	LocalCameraInstance::~LocalCameraInstance()
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

	LocalCameraInstance::INSTANCE_TYPE LocalCameraInstance::type() noexcept { return INSTANCE_CAMERA; }
	const char* LocalCameraInstance::name() noexcept { return ("LocalCameraInstance"); }

	void LocalCameraInstance::update()
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

	irr::scene::ICameraSceneNode* LocalCameraInstance::get() noexcept { return m_camera; }

	LocalWatchdogEvent::LocalWatchdogEvent()
		: m_network(nullptr)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		XPLICIT_ASSERT(m_network);
	}

	LocalWatchdogEvent::~LocalWatchdogEvent() 
	{
		
	}

	void LocalWatchdogEvent::operator()()
	{
		if (m_network)
		{
			auto& packet = m_network->get();

			if (packet.cmd[XPLICIT_NETWORK_CMD_WATCHDOG] == NETWORK_CMD_WATCHDOG)
			{
				packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
				m_network->send(packet);
			}

			if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (!InstanceManager::get_singleton_ptr()->get<XUI::ErrorMessage>("ErrorMessage"))
				{
					InstanceManager::get_singleton_ptr()->add<XUI::ErrorMessage>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::ERROR_TYPE::Kicked);

				}
			}
		}
	}
}