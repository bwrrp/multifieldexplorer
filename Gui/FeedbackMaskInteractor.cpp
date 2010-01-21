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
		lastPid = -1;
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
	bool FeedbackMaskInteractor::MouseMoveEvent(NQVTK::MouseEvent event)
	{
		if (currentFeature && 
			(event.buttons & NQVTK::MouseEvent::LeftButton) != 0)
		{
			// Find the property under the cursor
			int pid = static_cast<int>(
				static_cast<double>(event.x) * 
				static_cast<double>(currentFeature->mask.size()) / 
				static_cast<double>(viewportWidth));
			// Reset drag when the mouse leaves the viewer
			if (pid < 0)
			{
				lastPid = -1;
			}
			else if (pid > static_cast<int>(currentFeature->mask.size()) - 1)

			{
				lastPid = -1;
			}
			// Don't toggle the same property twice
			else if (pid != lastPid)
			{
				// Toggle the mask
				currentFeature->mask[pid] = !currentFeature->mask[pid];
				lastPid = pid;
				messenger->EmitUpdated();
				return true;
			}
		}

		return false;
	}

	// ------------------------------------------------------------------------
	bool FeedbackMaskInteractor::MouseReleaseEvent(NQVTK::MouseEvent event)
	{
		// Make sure the button involved in the event is considered active
		event.buttons |= event.button;

		bool res = MouseMoveEvent(event);

		if (event.button == NQVTK::MouseEvent::LeftButton)
		{
			// Allow a next click to toggle any property
			lastPid = -1;
		}

		return res;
	}
}
