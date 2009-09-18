#include "GLBlaat/GL.h"

#include "MainViewer.h"
#include "MainViewer.moc"

#include <NQVTK/Interactors/ArcballCameraInteractor.h>

#include <NQVTK/Rendering/ArcballCamera.h>
#include <NQVTK/Rendering/SimpleRenderer.h>

namespace VFE
{
	// ------------------------------------------------------------------------
	MainViewer::MainViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::SimpleRenderer *renderer = new NQVTK::SimpleRenderer();
		NQVTK::ArcballCamera *camera = new NQVTK::ArcballCamera();
		renderer->SetCamera(camera);
		SetRenderer(renderer);
		SetInteractor(new NQVTK::ArcballCameraInteractor(camera));
	}

	// ------------------------------------------------------------------------
	MainViewer::~MainViewer()
	{
	}
}
