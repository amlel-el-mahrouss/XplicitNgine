/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: XUI.h
 *			Purpose: Xplicit Resource based UI.
 *
 * =====================================================================
 */

#pragma once

#include <Application.h>
#include <Instance.h>

#ifdef XPLICIT_WINDOWS
#include "DriverD3D11.h"
#endif

namespace Xplicit::UI
{
	enum class POPUP_TYPE
	{
		Generic,
		NetworkError,
		Shutdown,
		Kicked,
	};

	class InternalPopup final : public Instance
	{
	public:
		InternalPopup() = delete;

	public:
		InternalPopup(std::function<void()> on_click, const vector2di pos = vector2di(0, 0), const POPUP_TYPE shutdown_type = POPUP_TYPE::NetworkError) noexcept;
		virtual ~InternalPopup();

		InternalPopup& operator=(const InternalPopup&) = delete;
		InternalPopup(const InternalPopup&) = delete;

		virtual void update() override;

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

	private:
		std::function<void()> m_on_click;
		ITexture* m_error_texture;
		vector2di m_pos;

	};
}