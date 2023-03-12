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

#include "App.h"

//
// creates an opengl device, alongside other things.
// that's it
//

INT32 WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		Xplicit::App::Application* app = new Xplicit::App::Application();

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
		MessageBoxA(nullptr, err.what(), "XplicitNgin, Error!", MB_OK);
#endif

		return -1;
	}
}
