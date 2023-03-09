#include "IDriverSystem.h"

namespace Xplicit::Renderer
{
	const char* IDriverSystem::name() noexcept { return ("IDriverSystem"); }
	RENDER_SYSTEM IDriverSystem::api() { return RENDER_SYSTEM::INVALID; }
}