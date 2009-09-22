#pragma once

#include <NQVTK/Styles/LayeredRaycaster.h>

#include <NQVTK/Math/Vector3.h>

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

		// Used for both the painter and the raycaster
		virtual void UpdatePainterParameters(GLProgram *painter);

		NQVTK::Vector3 cursorPos;
	};
}
