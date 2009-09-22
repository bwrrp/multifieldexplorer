#include "GLBlaat/GL.h"

#include "SliceViewer.h"
#include "SliceViewer.moc"

#include <NQVTK/Interactors/SliceViewInteractor.h>

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
		SetInteractor(new NQVTK::SliceViewInteractor(renderer));

		// TODO: create interactor and use crosshair for point selection
		toggleCrosshair(true);
		connect(this, SIGNAL(cursorPosChanged(double, double)), 
			this, SLOT(setCrosshairPos(double, double)));
		connect(this, SIGNAL(cursorPosChanged(double, double)), 
			this, SLOT(on_cursorPosChanged(double, double)));
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

	// ------------------------------------------------------------------------
	void SliceViewer::on_cursorPosChanged(double x, double y)
	{
		// Get plane parameters
		NQVTK::SliceRenderer *renderer = 
			dynamic_cast<NQVTK::SliceRenderer*>(GetRenderer());
		NQVTK::Vector3 origin = renderer->GetViewportPlaneOrigin();
		NQVTK::Vector3 right = renderer->GetViewportPlaneRight();
		NQVTK::Vector3 up = renderer->GetViewportPlaneUp();
		// Get 3D position of the cursor
		x = 0.5 * (1.0 + x / renderer->GetCamera()->aspect);
		y = 0.5 * (1.0 - y);
		// TODO: check this!
		NQVTK::Vector3 pos = origin + x * right + y * up;
		emit CursorPosChanged(pos);
	}
}
