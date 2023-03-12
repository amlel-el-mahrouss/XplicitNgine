/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave logic
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

		int64_t size() noexcept;
		virtual void operator()() override;
		virtual const char* name() noexcept override;

	private:
		bool on_leave() noexcept;
		bool on_join() noexcept;

	private:
		int64_t m_actor_counter;

	};
}