/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Client.cpp
 *			Purpose: Client sources
 *
 * =====================================================================
 */

#include "App.h"

static void xplicit_create_opengl()
{
	KB = new Xplicit::InputReceiver();

	if (!KB) throw std::bad_alloc();

	dimension2du dim2d = dimension2du(800, 600);

	Xplicit::Application::get_singleton().set(irr::createDevice(irr::video::EDT_OPENGL,
		dim2d,
		16U,
		false,
		false,
		false,
		KB));

	HMENU menuHandle = GetSystemMenu((HWND)IRR->getVideoDriver()->getExposedVideoData().OpenGLWin32.HWnd, FALSE);
	EnableMenuItem(menuHandle, SC_CLOSE, MF_GRAYED);

	IRR->setWindowCaption(Xplicit::App::XPLICIT_APP_NAME);

}

INT32 WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		xplicit_create_opengl();

		Xplicit::App::Application* app = new Xplicit::App::Application();

		while (IRR->run() && Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			IRR->getVideoDriver()->beginScene(true, true, irr::video::SColor(0, 38, 100, 100));

			IRR->getSceneManager()->drawAll();
			IRR->getGUIEnvironment()->drawAll();

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			IRR->getVideoDriver()->endScene();
		}

		return 0;
	}
	catch (...)
	{
		Xplicit::GUI::message_box(L"Uh oh!", L"Something wrong happened! We can't continue!", MB_OK);

		return -1;
	}
}
