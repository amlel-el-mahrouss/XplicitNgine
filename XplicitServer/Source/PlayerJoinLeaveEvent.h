/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class PlayerJoinLeaveEvent : public Event
	{
	public:
		PlayerJoinLeaveEvent();
		virtual ~PlayerJoinLeaveEvent();

		PlayerJoinLeaveEvent& operator=(const PlayerJoinLeaveEvent&) = default;
		PlayerJoinLeaveEvent(const PlayerJoinLeaveEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept override;

	private:
		bool on_leave(NetworkServerInstance* server) noexcept;
		bool on_join(NetworkServerInstance* server) noexcept;

	private:
		int64_t m_id_counter;

	};
}