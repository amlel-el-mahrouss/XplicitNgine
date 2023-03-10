#pragma once

#include <Foundation.h>

#ifdef XPLICIT_WINDOWS

#include <intrin.h>
#include <d3d11.h>

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#endif

#include <nuklear/nuklear.h>

// Our WIP renderer, to replace irrlicht.

namespace Xplicit::Renderer
{
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		INVALID,
		VULKAN,
	};

	class DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api();

	};
}