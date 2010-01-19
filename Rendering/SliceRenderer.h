#pragma once

#include <NQVTK/Rendering/SliceRenderer.h>

#include <string>

class GLProgram;

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

	protected:
		Field *field;

		virtual bool Initialize();

		GLProgram *CreateSliceViewerShader();
		std::string AddShaderDefines(const std::string &shader);
	};
}
