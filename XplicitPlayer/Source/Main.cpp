/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Main.cpp
 *			Purpose: Main unit.
 *
 * =====================================================================
 */

#include "Application.h"
#include "Uri.h"

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_WINDOWS
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Bites::XPLICIT_APP_NAME))
		{
			Xplicit::GUI::message_box(Xplicit::Bites::XPLICIT_APP_NAME, L"Another instance of XplicitNgin has been found! aborting..", MB_OK);
			return 1;
		}
#else
#error Undefined platform!
#endif


		Xplicit::Utils::UriParser uri;
		uri /= pCmdLine;

		if (inet_addr(uri.get().c_str()) == INADDR_NONE)
			return 1;

		Xplicit::Bites::Application* app = new Xplicit::Bites::Application(uri.get().c_str());
		if (!app) throw Xplicit::EngineError();

		while (IRR->run() && Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			// To match the loading's screen colour.
			IRR->getVideoDriver()->beginScene(true, true, irr::video::SColor(40, 40, 40, 40));

			IRR->getSceneManager()->drawAll();
			IRR->getGUIEnvironment()->drawAll();

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			IRR->getVideoDriver()->endScene();
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
#ifdef XPLICIT_WINDOWS
		MessageBoxA(nullptr, err.what(), "XplicitNgin", MB_OK);
#endif

		return -1;
	}
}
