#pragma once

#include <NQVTK/Styles/CustomLayeredRaycaster.h>

#include <NQVTK/Math/Vector3.h>

namespace MFE
{
	class Field;

	class SimiBlobStyle : public NQVTK::Styles::CustomLayeredRaycaster
	{
	public:
		typedef NQVTK::Styles::CustomLayeredRaycaster Superclass;

		SimiBlobStyle();
		~SimiBlobStyle();

		// Scribe stage peeling pass
		virtual GLProgram *CreateScribe();
		// Scribe stage raycasting pass
		virtual GLProgram *CreateRaycaster();
		// Painter stage
		virtual GLProgram *CreatePainter();

		// Used for both the painter and the raycaster
		virtual void UpdatePainterParameters(GLProgram *painter);

		void SetField(const Field *field);

	protected:
		const Field *field;
	};
}
