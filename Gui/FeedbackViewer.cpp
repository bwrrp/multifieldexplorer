#include "FeedbackViewer.h"
#include "FeedbackViewer.moc"

#include "Rendering/FeedbackRenderer.h"

#include <cassert>

namespace MFE
{
	// ------------------------------------------------------------------------
	FeedbackViewer::FeedbackViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		FeedbackRenderer *renderer = new FeedbackRenderer();
		SetRenderer(renderer);
	}
	
	// ------------------------------------------------------------------------
	FeedbackViewer::~FeedbackViewer()
	{
	}

	// ------------------------------------------------------------------------
	void FeedbackViewer::SetField(Field *field)
	{
		FeedbackRenderer *renderer = 
			dynamic_cast<FeedbackRenderer*>(GetRenderer());
		assert(renderer != 0);

		renderer->SetField(field);
	}

	// ------------------------------------------------------------------------
	void FeedbackViewer::SetFeature(Feature *feature)
	{
		FeedbackRenderer *renderer = 
			dynamic_cast<FeedbackRenderer*>(GetRenderer());
		assert(renderer != 0);

		renderer->SetFeature(feature);
	}
};
