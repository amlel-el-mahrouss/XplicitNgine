/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct 3D 11
 *
 * =====================================================================
 */

#pragma once

#include "DriverSystem.h"

#ifdef XPLICIT_WINDOWS

#include <intrin.h>
#include <d3d11.h>

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#else

#error You need Vulkan support!

#endif

#include <nuklear/nuklear.h>
