#pragma once

#include <NQVTK/Rendering/SliceRenderer.h>

namespace VFE
{
	class VectorField;

	class SliceRenderer : public NQVTK::SliceRenderer
	{
	public:
		typedef NQVTK::SliceRenderer Superclass;

		SliceRenderer();

		void Draw();

		void SceneChanged();

		void SetField(VectorField *field);

		float kernelSize;

	protected:
		VectorField *field;

		float unitSize;

		virtual bool Initialize();
	};
}
