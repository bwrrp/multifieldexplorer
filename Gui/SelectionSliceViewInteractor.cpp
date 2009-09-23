#include "SelectionSliceViewInteractor.h"
#include "SelectionSliceViewInteractor.moc"

#include <NQVTK/Rendering/SliceRenderer.h>

namespace VFE
{
	// ------------------------------------------------------------------------
	void SelectionMessenger::EmitCursorPosChanged(const NQVTK::Vector3 &pos)
	{
		emit CursorPosChanged(pos);
	}

	// ------------------------------------------------------------------------
	void SelectionMessenger::EmitPointSelected(const NQVTK::Vector3 &pos)
	{
		emit PointSelected(pos);
	}

	// ------------------------------------------------------------------------
	SelectionSliceViewInteractor::SelectionSliceViewInteractor(
		NQVTK::SliceRenderer *renderer)
		: NQVTK::SliceViewInteractor(renderer)
	{
		messenger = new SelectionMessenger();
	}

	// ------------------------------------------------------------------------
	SelectionSliceViewInteractor::~SelectionSliceViewInteractor()
	{
		delete messenger;
	}

	// ------------------------------------------------------------------------
	bool SelectionSliceViewInteractor::MouseMoveEvent(NQVTK::MouseEvent event)
	{
		bool res = Superclass::MouseMoveEvent(event);
		messenger->EmitCursorPosChanged(Get3DPos(event));
		return res;
	}

	// ------------------------------------------------------------------------
	bool SelectionSliceViewInteractor::MouseReleaseEvent(
		NQVTK::MouseEvent event)
	{
		bool res = Superclass::MouseReleaseEvent(event);
		messenger->EmitPointSelected(Get3DPos(event));
		return res;
	}

	// ------------------------------------------------------------------------
	NQVTK::Vector3 SelectionSliceViewInteractor::Get3DPos(
		const NQVTK::MouseEvent &event)
	{
		// Get viewport-relative coordinates
		double x = static_cast<double>(event.x) / 
			static_cast<double>(viewportWidth);
		double y = 1.0 - static_cast<double>(event.y) / 
			static_cast<double>(viewportHeight);
		// Get plane parameters
		NQVTK::Vector3 origin = renderer->GetViewportPlaneOrigin();
		NQVTK::Vector3 right = renderer->GetViewportPlaneRight();
		NQVTK::Vector3 up = renderer->GetViewportPlaneUp();
		// Compute 3D position
		return origin + x * right + y * up;
	}
}
