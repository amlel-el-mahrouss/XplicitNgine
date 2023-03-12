/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerEvent.h
 *			Purpose: Player Events
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class PlayerManager final
	{
	private:
		PlayerManager() = default;

	public:
		~PlayerManager() = default;

		PlayerManager& operator=(const PlayerManager&) = default;
		PlayerManager(const PlayerManager&) = default;

	public:
		static std::unique_ptr<PlayerManager>& get_singleton_ptr() noexcept;

	public:
		void operator++() { ++m_players; }
		void operator--() { --m_players; }

	public:
		const int64_t& size() noexcept;

	private:
		int64_t m_players;
		std::string m_host;

	};

	class PlayerJoinEvent : public Event
	{
	public:
		PlayerJoinEvent() = default;
		virtual ~PlayerJoinEvent() = default;

		PlayerJoinEvent& operator=(const PlayerJoinEvent&) = default;
		PlayerJoinEvent(const PlayerJoinEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept override;

	};


	class PlayerLeaveEvent : public Event
	{
	public:
		PlayerLeaveEvent() = default;
		virtual ~PlayerLeaveEvent() = default;

		PlayerLeaveEvent& operator=(const PlayerLeaveEvent&) = default;
		PlayerLeaveEvent(const PlayerLeaveEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept override;

	};
}