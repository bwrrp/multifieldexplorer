#include "FeedbackMaskInteractor.h"
#include "FeedbackMaskInteractor.moc"

#include "Data/Feature.h"

#include <cmath>

namespace MFE
{
	// ------------------------------------------------------------------------
	void MaskMessenger::EmitUpdated()
	{
		emit Updated();
	}

	// ------------------------------------------------------------------------
	FeedbackMaskInteractor::FeedbackMaskInteractor() : currentFeature(0)
	{
		messenger = new MaskMessenger();
	}

	// ------------------------------------------------------------------------
	FeedbackMaskInteractor::~FeedbackMaskInteractor()
	{
		delete messenger;
	}

	// ------------------------------------------------------------------------
	void FeedbackMaskInteractor::SetFeature(Feature *feature)
	{
		currentFeature = feature;
	}

	// ------------------------------------------------------------------------
	bool FeedbackMaskInteractor::MouseReleaseEvent(NQVTK::MouseEvent event)
	{
		if (currentFeature)
		{
			// Find the property under the cursor
			int pid = static_cast<int>(
				static_cast<double>(event.x * currentFeature->mask.size()) / 
				static_cast<double>(viewportWidth));
			// Toggle the mask
			currentFeature->mask[pid] = !currentFeature->mask[pid];

			messenger->EmitUpdated();
			return true;
		}

		return false;
	}
}
