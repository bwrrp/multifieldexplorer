#include "GLBlaat/GL.h"

#include "MainViewer.h"
#include "MainViewer.moc"

#include <NQVTK/Rendering/SimpleRenderer.h>

namespace VFE
{
	// ------------------------------------------------------------------------
	MainViewer::MainViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		SetRenderer(new NQVTK::SimpleRenderer());
	}

	// ------------------------------------------------------------------------
	MainViewer::~MainViewer()
	{
	}
}
