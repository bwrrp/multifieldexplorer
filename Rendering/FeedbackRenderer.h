#pragma once

#include <NQVTK/Rendering/Renderer.h>

#include <string>

class GLProgram;

namespace MFE
{
	class DataTransform;
	class Field;
	class Feature;

	class FeedbackRenderer : public NQVTK::Renderer
	{
	public:
		typedef NQVTK::Renderer Superclass;

		FeedbackRenderer();
		virtual ~FeedbackRenderer();

		void Draw();

		void SetField(Field *field);
		void SetFeature(Feature *feature);

	protected:
		virtual bool Initialize();

		std::string AddShaderDefines(const std::string &shader);

		Feature *currentFeature;

		GLProgram *shader;

		DataTransform *transform;
	};
}
