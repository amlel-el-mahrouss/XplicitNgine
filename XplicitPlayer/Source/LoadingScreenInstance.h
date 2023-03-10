/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingSreenInstance.h
 *			Purpose: Client Loading Screen
 *
 * =====================================================================
 */

#pragma once

#include <Application.h>
#include <NetworkInstance.h>

namespace Xplicit::Client
{
	class LoadingScreenInstance : public Instance
	{
	public:
		LoadingScreenInstance();
		virtual ~LoadingScreenInstance();

		LoadingScreenInstance& operator=(const LoadingScreenInstance&) = default;
		LoadingScreenInstance(const LoadingScreenInstance&) = default;

		virtual void update() override;
		void connect(const char* ip);

	private:
		NetworkInstance* m_net;
		int64_t m_timeout;
		bool m_connected;

	};
}