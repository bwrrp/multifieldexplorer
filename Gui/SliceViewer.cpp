#include "GLBlaat/GL.h"

#include "SliceViewer.h"
#include "SliceViewer.moc"

#include <NQVTK/Rendering/SliceRenderer.h>
#include <NQVTK/Interactors/SliceViewInteractor.h>

namespace VFE
{
	// ------------------------------------------------------------------------
	SliceViewer::SliceViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::SliceRenderer *renderer = new NQVTK::SliceRenderer();
		SetRenderer(renderer);
		SetInteractor(new NQVTK::SliceViewInteractor(renderer));
	}

	// ------------------------------------------------------------------------
	SliceViewer::~SliceViewer()
	{
	}
}
