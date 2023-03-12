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

#include "XUI.h"
#include "App.h"

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
		return "NetworkPopup";
	}

	ErrorMessage::INSTANCE_TYPE ErrorMessage::type() noexcept
	{
		return ErrorMessage::INSTANCE_GUI;
	}
	
	HUD::HUD()
	{
		XPLICIT_GET_DATA_DIR(dat);

		std::string full_health_path = dat;
		std::string mid_health_path = dat;
		std::string empty_health_path = dat;

		// precache the health textures
		full_health_path += "\\Textures\\health_bar_full.png";
		m_full_heatlh = IRR->getVideoDriver()->getTexture(full_health_path.c_str());

		mid_health_path += "\\Textures\\health_bar_low.png";
		m_mid_heatlh = IRR->getVideoDriver()->getTexture(full_health_path.c_str());

		empty_health_path += "\\Textures\\health_bar_empty.png";
		m_no_heatlh = IRR->getVideoDriver()->getTexture(full_health_path.c_str());

		if (!m_mid_heatlh || !m_full_heatlh || !m_no_heatlh)
			throw EngineError();

		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (!m_network)
			throw EngineError();
	}

	HUD::~HUD()
	{
		if (m_no_heatlh)
			m_no_heatlh->drop();

		if (m_mid_heatlh)
			m_mid_heatlh->drop();

		if (m_full_heatlh)
			m_full_heatlh->drop();
	}

	void HUD::update()
	{
		auto packet = m_network->get();
		
		for (size_t i = XPLICIT_LAST_RESERVED_CMD; i < XPLICIT_NETWORK_MAX_CMDS; ++i)
		{
			if (packet.CMD[i] == NETWORK_CMD_DAMAGE)
				m_health = packet.Health;
		}

		auto dim = dimension2di(10, 10);
		auto sz = core::rect<s32>(0, 0, 146, 22);

		if (m_health > 50)
		{
			IRR->getVideoDriver()->draw2DImage(m_full_heatlh, dim,
				sz, 0,
				video::SColor(255, 255, 255, 255), true);
		}
		else if (m_health < 50)
		{
			IRR->getVideoDriver()->draw2DImage(m_mid_heatlh, dim,
				sz, 0,
				video::SColor(255, 255, 255, 255), true);
		}
		else if (m_health < 0)
		{
			IRR->getVideoDriver()->draw2DImage(m_no_heatlh, dim,
				sz, 0,
				video::SColor(255, 255, 255, 255), true);
		}
	}

}