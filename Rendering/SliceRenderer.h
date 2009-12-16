#pragma once

#include <NQVTK/Rendering/SliceRenderer.h>

namespace MFE
{
	class Field;

	class SliceRenderer : public NQVTK::SliceRenderer
	{
	public:
		typedef NQVTK::SliceRenderer Superclass;

		SliceRenderer();

		void Draw();

		void SceneChanged();

		void SetField(Field *field);

		float kernelSize;

	protected:
		Field *field;

		float unitSize;

		virtual bool Initialize();
	};
}
