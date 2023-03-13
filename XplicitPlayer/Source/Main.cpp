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

//
// creates an opengl device, alongside other things.
// that's it
//

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	try
	{
		Xplicit::Bites::Application* app = new Xplicit::Bites::Application("10.211.55.9");
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
