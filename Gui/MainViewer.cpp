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
		NQVTK::SimpleRenderer *ren = new NQVTK::SimpleRenderer();
		NQVTK::ArcballCamera *cam = new NQVTK::ArcballCamera();
		ren->SetCamera(cam);
		SetRenderer(ren);
		SetInteractor(new NQVTK::ArcballCameraInteractor(cam));
	}

	// ------------------------------------------------------------------------
	MainViewer::~MainViewer()
	{
	}
}
