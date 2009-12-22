#include "FeedbackRenderer.h"

namespace MFE
{
	// ------------------------------------------------------------------------
	FeedbackRenderer::FeedbackRenderer() : currentFeature(0)
	{
	}

	// ------------------------------------------------------------------------
	FeedbackRenderer::~FeedbackRenderer()
	{
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::Draw()
	{
		Clear();

		if (currentFeature)
		{
			// Projection pass

			// TODO: Set up shader, transform texture
			// TODO: Pass feature example pos & bias vector type
			// TODO: Render to FBO

			// Visualization pass (color mapping)

			// TODO: Set up shader, min/max texture
			// TODO: Render to screen
		}
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::SetField(Field *field)
	{
		// TODO: create or update the transform and min/max textures
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::SetFeature(Feature *feature)
	{
		currentFeature = feature;
	}

	// ------------------------------------------------------------------------
	bool FeedbackRenderer::Initialize()
	{
		if (!Superclass::Initialize()) return false;

		// TODO: create and initialize FBO for the projected example / vector
		// TODO: create shaders for projection and color mapping
	}
}
