#pragma once

#include <NQVTK/Styles/LayeredRaycaster.h>

namespace VFE
{
	class VectorNPRStyle : public NQVTK::Styles::LayeredRaycaster
	{
	public:
		typedef NQVTK::Styles::LayeredRaycaster Superclass;

		VectorNPRStyle();
		~VectorNPRStyle();

		// Scribe stage peeling pass
		virtual GLProgram *CreateScribe();
		// Scribe stage raycasting pass
		virtual GLProgram *CreateRaycaster();
		// Painter stage
		virtual GLProgram *CreatePainter();
	};
}
