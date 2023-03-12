/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalInstance.cpp
 *			Purpose: Client Camera
 *
 * =====================================================================
 */

#include "LocalInstance.h"

namespace Xplicit::Client
{
	LocalInstance::LocalInstance()
		: Instance(), m_camera(nullptr), m_idle_tex(nullptr), 
		m_moving_tex(nullptr), m_typing_tex(nullptr),
		m_network(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeFPS(nullptr, 60.F, 0.2F);
		IRR->getCursorControl()->setVisible(false);

		m_camera->setName("LocalInstance");

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

		// ask for the network instance.

		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
	}

	LocalInstance::~LocalInstance()
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

	LocalInstance::INSTANCE_TYPE LocalInstance::type() noexcept { return INSTANCE_CAMERA; }
	const char* LocalInstance::name() noexcept { return ("LocalInstance"); }

	void LocalInstance::update()
	{
		if (!m_network)
			IRR->closeDevice();

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

		if (KB->key_down(irr::KEY_CONTROL) || KB->key_down(KEY_KEY_L))
		{
			// send a stop command.
			NetworkPacket cmd = {  };
			cmd.CMD[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP; // client is shutting down.

			m_network->send(cmd);

			IRR->closeDevice();
		}
	}

	irr::scene::ICameraSceneNode* LocalInstance::get() noexcept { return m_camera; }

	LocalWatchdogEvent::LocalWatchdogEvent()
		: m_network(nullptr)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);
	}

	LocalWatchdogEvent::~LocalWatchdogEvent() 
	{
		
	}

	void LocalWatchdogEvent::operator()()
	{
		if (m_network)
		{
			auto& packet = m_network->get();

			if (packet.CMD[XPLICIT_NETWORK_CMD_WATCHDOG] == NETWORK_CMD_WATCHDOG)
			{
				packet.CMD[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
				m_network->send(packet);
			}
		}
	}
}