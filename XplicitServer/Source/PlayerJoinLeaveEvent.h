/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX Technologies all rights reserved.
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
	/*
	* 
	*	The Purpose of this class is to handle Join and Leave connections
	*	
	*/
	class PlayerJoinLeaveEvent : public Event
	{
	public:
		PlayerJoinLeaveEvent();
		virtual ~PlayerJoinLeaveEvent();

		PlayerJoinLeaveEvent& operator=(const PlayerJoinLeaveEvent&) = default;
		PlayerJoinLeaveEvent(const PlayerJoinLeaveEvent&) = default;

		virtual void operator()() override;

	private:
		bool on_leave() noexcept;
		bool on_join() noexcept;

		virtual const char* name() noexcept override;

	private:
		int64_t m_id_counter;

	};
}