#pragma once

#include <NQVTK/Styles/CustomLayeredRaycaster.h>

#include <NQVTK/Math/Vector3.h>

namespace VFE
{
	class VectorNPRStyle : public NQVTK::Styles::CustomLayeredRaycaster
	{
	public:
		typedef NQVTK::Styles::CustomLayeredRaycaster Superclass;

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
		NQVTK::Vector3 selectedPos;
	};
}
