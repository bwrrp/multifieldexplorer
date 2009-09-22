#include "GLBlaat/GL.h"

#include "MainViewer.h"
#include "MainViewer.moc"

#include <NQVTK/Interactors/ArcballCameraInteractor.h>

#include <NQVTK/Rendering/ArcballCamera.h>
#include <NQVTK/Rendering/LayeredRaycastingRenderer.h>

#include "Rendering/VectorNPRStyle.h"

namespace VFE
{
	// ------------------------------------------------------------------------
	MainViewer::MainViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::LayeredRaycastingRenderer *renderer = 
			new NQVTK::LayeredRaycastingRenderer();

		style = new VectorNPRStyle();
		renderer->SetStyle(style);

		NQVTK::ArcballCamera *camera = new NQVTK::ArcballCamera();
		renderer->SetCamera(camera);

		SetRenderer(renderer);
		SetInteractor(new NQVTK::ArcballCameraInteractor(camera));
	}

	// ------------------------------------------------------------------------
	MainViewer::~MainViewer()
	{
	}

	// ------------------------------------------------------------------------
	void MainViewer::SetCursorPos(NQVTK::Vector3 pos)
	{
		style->cursorPos = pos;
		updateGL();
	}
}
