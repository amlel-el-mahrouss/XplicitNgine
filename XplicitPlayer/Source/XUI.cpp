/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XUI.cpp
 *			Purpose: Xplicit Resource based UI.
 *
 * =====================================================================
 */

#include "Application.h"
#include "XUI.h"

namespace Xplicit::XUI
{
	ErrorMessage::ErrorMessage(std::function<void()> on_click, const vector2di pos, const ERROR_TYPE shutdown_type) noexcept
		: m_on_click(on_click), m_pos(pos)
	{
		assert(on_click);

		XPLICIT_GET_DATA_DIR(dat);
		std::string path = dat;

		switch (shutdown_type)
		{
		case ERROR_TYPE::Generic:
			path += "\\Textures\\internal_error.png";
			break;
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

		m_error_texture = IRR->getVideoDriver()->getTexture(path.c_str());
		assert(m_error_texture);

		if (!m_error_texture)
			throw EngineError();
	}

	ErrorMessage::~ErrorMessage()
	{
		if (m_error_texture)
			m_error_texture->drop();
	}

	void ErrorMessage::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_error_texture, m_pos);

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
		: m_health(0), m_health_bar(nullptr),  m_network(nullptr)
	{
		XPLICIT_GET_DATA_DIR(dat);

		std::string health_path = dat;

		// precache the health textures
		health_path += "\\Textures\\health_bar.png";
		m_health_bar = IRR->getVideoDriver()->getTexture(health_path.c_str());

		XPLICIT_ASSERT(m_health_bar);

		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		XPLICIT_ASSERT(m_network);
	}

	HUD::~HUD()
	{
		if (m_health_bar)
			m_health_bar->drop();
	}

	void HUD::update()
	{
		auto packet = m_network->get();
		
		for (size_t i = XPLICIT_LAST_RESERVED_CMD; i < XPLICIT_NETWORK_CMD_MAX; ++i)
		{
			if (packet.cmd[i] == NETWORK_CMD_DAMAGE)
				m_health = packet.health;
		}

		auto dim = dimension2di(10, 10);
		auto sz = core::rect<s32>(0, 0, 146, 22);

		IRR->getVideoDriver()->draw2DImage(m_health_bar, dim,
			sz, 0,
			video::SColor(255, 255, 255, 255), true);
	}

}