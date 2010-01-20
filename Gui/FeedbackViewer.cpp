#include "FeedbackViewer.h"
#include "FeedbackViewer.moc"

#include "Rendering/FeedbackRenderer.h"

#include "FeedbackMaskInteractor.h"

#include <cassert>

namespace MFE
{
	// ------------------------------------------------------------------------
	FeedbackViewer::FeedbackViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		FeedbackRenderer *renderer = new FeedbackRenderer();
		SetRenderer(renderer);

		FeedbackMaskInteractor *interactor = new FeedbackMaskInteractor();
		connect(interactor->GetMessenger(), SIGNAL(Updated()), 
			this, SIGNAL(Updated()));
		SetInteractor(interactor);
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

		SetFeature(0);
	}

	// ------------------------------------------------------------------------
	void FeedbackViewer::SetFeature(Feature *feature)
	{
		FeedbackRenderer *renderer = 
			dynamic_cast<FeedbackRenderer*>(GetRenderer());
		assert(renderer != 0);
		renderer->SetFeature(feature);

		FeedbackMaskInteractor *interactor = 
			dynamic_cast<FeedbackMaskInteractor*>(GetInteractor());
		assert(interactor != 0);
		interactor->SetFeature(feature);
	}
};
