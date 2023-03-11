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

namespace Xplicit::UI
{
	InternalPopup::InternalPopup(std::function<void()> on_click, const vector2di pos, const POPUP_TYPE shutdown_type) noexcept
		: m_on_click(on_click), m_pos(pos)
	{
		assert(on_click);

		XPLICIT_GET_DATA_DIR(dat);
		std::string path = dat;

		switch (shutdown_type)
		{
		case POPUP_TYPE::Generic:
			path += "\\Textures\\internal_error.png";
			break;
		case POPUP_TYPE::Kicked:
			path += "\\Textures\\network_kicked.png";
			break;
		case POPUP_TYPE::NetworkError:
			path += "\\Textures\\network_error.png";
			break;
		case POPUP_TYPE::Shutdown:
			path += "\\Textures\\network_shutdown.png";

			break;
		}

		XPLICIT_INFO(path);

		m_error_texture = IRR->getVideoDriver()->getTexture(path.c_str());
		assert(m_error_texture);

		if (!m_error_texture)
			throw EngineError();
	}

	InternalPopup::~InternalPopup()
	{
		if (m_error_texture)
			m_error_texture->drop();
	}

	void InternalPopup::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_error_texture, m_pos);

		if (KB->left_down())
		{
			m_on_click();
		}
	}


	const char* InternalPopup::name() noexcept
	{
		return "NetworkPopup";
	}

	InternalPopup::INSTANCE_TYPE InternalPopup::type() noexcept
	{
		return InternalPopup::INSTANCE_GUI;
	}
	
	InternalHUD::InternalHUD()
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

	InternalHUD::~InternalHUD()
	{
		if (m_no_heatlh)
			m_no_heatlh->drop();

		if (m_mid_heatlh)
			m_mid_heatlh->drop();

		if (m_full_heatlh)
			m_full_heatlh->drop();
	}

	void InternalHUD::update()
	{
		auto packet = m_network->get();
		if (packet.CMD == NETWORK_CMD_DAMAGE)
			m_health = packet.Health;

		auto dim = dimension2di(Client::XPLICIT_DIM.Width / 3.5, Client::XPLICIT_DIM.Height * 0.82);
		auto sz = core::rect<s32>(0, 0, 521, 51);

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