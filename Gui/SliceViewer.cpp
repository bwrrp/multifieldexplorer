#include <GLBlaat/GL.h>
#include "SliceViewer.h"
#include "SliceViewer.moc"

#include "SelectionSliceViewInteractor.h"

#include <NQVTK/Math/Vector3.h>

#include <NQVTK/Rendering/Camera.h>
#include <NQVTK/Rendering/SliceRenderer.h>


namespace VFE
{
	// ------------------------------------------------------------------------
	SliceViewer::SliceViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::SliceRenderer *renderer = new NQVTK::SliceRenderer();
		renderer->SetPlane(
			NQVTK::Vector3(-10.0, -10.0, 5.0), 
			NQVTK::Vector3(20.0, 0.0, 0.0), 
			NQVTK::Vector3(0.0, 20.0, 0.0));
		SetRenderer(renderer);
		SelectionSliceViewInteractor *interactor = 
            new SelectionSliceViewInteractor(renderer);
		SetInteractor(interactor);
		// Display crosshair
		toggleCrosshair(true);
		connect(this, SIGNAL(cursorPosChanged(double, double)), 
			this, SLOT(setCrosshairPos(double, double)));
		// Expose interactor events
		connect(interactor->GetMessenger(), 
			SIGNAL(CursorPosChanged(NQVTK::Vector3)), 
			this, SIGNAL(CursorPosChanged(NQVTK::Vector3)));
		connect(interactor->GetMessenger(), 
			SIGNAL(PointSelected(NQVTK::Vector3)), 
			this, SIGNAL(PointSelected(NQVTK::Vector3)));
	}

	// ------------------------------------------------------------------------
	SliceViewer::~SliceViewer()
	{
	}

    // ------------------------------------------------------------------------
	void SliceViewer::initializeGL()
	{
		Superclass::initializeGL();

		NQVTK::SliceRenderer *renderer = 
			dynamic_cast<NQVTK::SliceRenderer*>(GetRenderer());
		if (!renderer->CreateDefaultShader())
		{
			qDebug("Could not create slice viewer shader!");
		}
	}
}
