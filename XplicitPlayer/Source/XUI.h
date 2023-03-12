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

namespace Xplicit::XUI
{
	enum class POPUP_TYPE
	{
		Generic,
		NetworkError,
		Shutdown,
		Kicked,
	};

	class Popup final : public Instance
	{
	public:
		Popup() = delete;

	public:
		Popup(std::function<void()> on_click, const vector2di pos = vector2di(0, 0), const POPUP_TYPE shutdown_type = POPUP_TYPE::NetworkError) noexcept;
		virtual ~Popup();

		Popup& operator=(const Popup&) = delete;
		Popup(const Popup&) = delete;

		virtual void update() override;

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

	private:
		std::function<void()> m_on_click;
		ITexture* m_error_texture;
		vector2di m_pos;

	};

	class HUD final : public Instance
	{
	public:
		HUD();
		virtual ~HUD();

		HUD& operator=(const HUD&) = delete;
		HUD(const HUD&) = delete;

		virtual const char* name() noexcept override { return "HUD"; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override;

	private:
		NetworkInstance* m_network;
		int64_t m_health;

	private:
		ITexture* m_full_heatlh;
		ITexture* m_mid_heatlh;
		ITexture* m_no_heatlh;

	};
}