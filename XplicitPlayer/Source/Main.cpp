/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX Technologies all rights reserved.
 *
 *			File: Client.cpp
 *			Purpose: Client sources
 *
 * =====================================================================
 */

#include "App.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		KB = new Xplicit::InputReceiver();

		if (!KB) throw std::bad_alloc();

		dimension2du dim2d = dimension2du(800, 600);

		Xplicit::Application::get_singleton().set(irr::createDevice(irr::video::EDT_DIRECT3D9,
			dim2d,
			16U,
			false,
			false,
			false,
			KB));

		IRR->setWindowCaption(Xplicit::App::XPLICIT_APP_NAME_WIDE);

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
		Xplicit::GUI::message_box(L"Uh oh!", L"We can't continue! Sorry!", MB_OK);

		return -1;
	}
}
