#pragma once

#include <NQVTK/Rendering/Renderer.h>

class GLProgram;

namespace MFE
{
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

		Feature *currentFeature;

		GLProgram *shader;

		int dimOriginal;
		int dimReduced;
	};
}
