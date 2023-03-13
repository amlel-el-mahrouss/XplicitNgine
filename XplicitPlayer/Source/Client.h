/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Client.h
 *			Purpose: Client Logic
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkInstance.h>

namespace Xplicit::Client
{
	class LoadingInstance : public Instance
	{
	public:
		LoadingInstance();
		virtual ~LoadingInstance();

		LoadingInstance& operator=(const LoadingInstance&) = default;
		LoadingInstance(const LoadingInstance&) = default;

		virtual void update() override;
		void connect(const char* ip);

		// resets the timeout and run..
		// in case of a watchdog..
		void reset() noexcept;

	private:
		irr::video::ITexture* m_logo_tex; /* Texture to show when loading the game.. */
		NetworkInstance* m_network; /* Network instance */
		int64_t m_timeout;
		bool m_run; /* Should we seek for a connection? */

	};

	class XPLICIT_API LocalResetEvent : public Event
	{
	public:
		LocalResetEvent();
		virtual ~LocalResetEvent();

		LocalResetEvent& operator=(const LocalResetEvent&) = default;
		LocalResetEvent(const LocalResetEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		int32_t m_num_resets;

	};

	class XPLICIT_API LocalMenuEvent : public Event
	{
	public:
		LocalMenuEvent(const int64_t& id);
		virtual ~LocalMenuEvent();

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

		bool enable(const bool enable = false) noexcept;
		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		int64_t m_timeout;
		ITexture* m_menu;
		bool m_enabled;
		int64_t m_hash;

	};
}