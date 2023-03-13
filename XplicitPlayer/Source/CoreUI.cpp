/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: CoreUI.cpp
 *			Purpose: Core Client UI.
 *
 * =====================================================================
 */

#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::CoreUI
{
	ErrorMessage::ErrorMessage(std::function<void()> on_click, const vector2di pos, const ERROR_TYPE shutdown_type) noexcept
		: m_on_click(on_click), m_pos(pos)
	{
		XPLICIT_ASSERT(on_click);

		XPLICIT_GET_DATA_DIR(dat);
		std::string path = dat;

		switch (shutdown_type)
		{
		case ERROR_TYPE::Kicked:
			path += "\\Textures\\network_kicked.png";
			break;
		case ERROR_TYPE::NetworkError:
			path += "\\Textures\\network_error.png";
			break;
		case ERROR_TYPE::Shutdown:
			path += "\\Textures\\network_shutdown.png";

			break;
		}

		XPLICIT_INFO(path);

		m_texture = IRR->getVideoDriver()->getTexture(path.c_str());
		assert(m_texture);

		if (!m_texture)
			throw EngineError();
	}

	ErrorMessage::~ErrorMessage()
	{
		if (m_texture)
			m_texture->drop();
	}

	void ErrorMessage::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_texture, m_pos);

		if (KB->key_down(KEY_ESCAPE))
		{
			m_on_click();
		}
	}


	const char* ErrorMessage::name() noexcept
	{
		return "ErrorMessage";
	}

	ErrorMessage::INSTANCE_TYPE ErrorMessage::type() noexcept
	{
		return ErrorMessage::INSTANCE_GUI;
	}
	
	HUD::HUD()
		: m_health(0), m_network(nullptr)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		XPLICIT_ASSERT(m_network);
	}

	HUD::~HUD()
	{

	}

	void HUD::update()
	{
		auto packet = m_network->get();
		
		for (size_t i = XPLICIT_LAST_RESERVED_CMD; i < XPLICIT_NETWORK_CMD_MAX; ++i)
		{
			if (packet.cmd[i] == NETWORK_CMD_DAMAGE)
				m_health = packet.health;
		}

		// TODO: draw health
	}

}