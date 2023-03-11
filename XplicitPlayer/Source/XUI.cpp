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
}