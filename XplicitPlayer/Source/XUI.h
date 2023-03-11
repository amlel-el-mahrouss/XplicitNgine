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

#include <NetworkInstance.h>
#include <ApplicationContext.h>
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

	class InternalHUD final : public Instance
	{
	public:
		InternalHUD();
		virtual ~InternalHUD();

		InternalHUD& operator=(const InternalHUD&) = delete;
		InternalHUD(const InternalHUD&) = delete;

		virtual const char* name() noexcept override { return "InternalHUD"; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override;

	private:
		NetworkInstance* m_network;
		ITexture* m_full_heatlh;
		ITexture* m_mid_heatlh;
		ITexture* m_no_heatlh;
		int64_t m_health;

	};
}