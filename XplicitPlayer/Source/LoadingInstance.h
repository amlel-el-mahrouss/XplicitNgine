/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingSreenInstance.h
 *			Purpose: Client Loading Logic
 *
 * =====================================================================
 */

#pragma once

#include <Application.h>
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

	private:
		irr::video::ITexture* m_logo_tex; /* Texture to show when loading the game.. */
		NetworkInstance* m_network; /* Network instance */
		bool m_connected; /* Are we connected? */

	};
}