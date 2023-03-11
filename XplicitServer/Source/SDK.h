/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: SDK.h
 *			Purpose: Xplicit SDK Headers
 *
 * =====================================================================
 */

#pragma once

#include <Actor.h>
#include <Event.h>
#include <Instance.h>
#include <Foundation.h>
#include <MonoInterop.h>
#include <Application.h>
#include <MonoInstance.h>
#include <NetworkServerInstance.h>

#ifdef XPLICIT_WINDOWS
extern "C"
{
#	include <conio.h>
};
#endif