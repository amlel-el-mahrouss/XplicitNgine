/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: ServerWatchdog.h
 *			Purpose: Server Watchdog
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	/*
	 *
	 * The purpose of this class is to watchdog for inactive connections
	 * 
	 */

	class ServerWatchdogEvent : public Event
	{
	public:
		ServerWatchdogEvent();
		virtual ~ServerWatchdogEvent();

		ServerWatchdogEvent& operator=(const ServerWatchdogEvent&) = default;
		ServerWatchdogEvent(const ServerWatchdogEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept override;

	public:
		operator bool();
		void set(bool enable) noexcept;

	private:
		bool m_watchdog_analyze; // should we analyse packets now?
		bool m_watchdog_enabled; // is watchdog enabled?

	};
}