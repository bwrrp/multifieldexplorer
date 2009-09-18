#include "GLBlaat/GL.h"

#include "SliceViewer.h"
#include "SliceViewer.moc"

#include <NQVTK/Interactors/SliceViewInteractor.h>

#include <NQVTK/Math/Vector3.h>

#include <NQVTK/Rendering/SliceRenderer.h>


namespace VFE
{
	// ------------------------------------------------------------------------
	SliceViewer::SliceViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::SliceRenderer *renderer = new NQVTK::SliceRenderer();
		renderer->SetPlane(
			NQVTK::Vector3(-10, -10, 0), 
			NQVTK::Vector3(20, 0, 0), 
			NQVTK::Vector3(0, 20, 0));
		SetRenderer(renderer);
		SetInteractor(new NQVTK::SliceViewInteractor(renderer));
	}

	// ------------------------------------------------------------------------
	SliceViewer::~SliceViewer()
	{
	}
}
