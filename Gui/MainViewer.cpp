#include <GLBlaat/GL.h>
#include "MainViewer.h"
#include "MainViewer.moc"

#include <NQVTK/Interactors/ArcballCameraInteractor.h>

#include <NQVTK/Rendering/ArcballCamera.h>
#include <NQVTK/Rendering/LayeredRaycastingRenderer.h>

#include "Data/Field.h"

#include "Rendering/SimiBlobStyle.h"

namespace MFE
{
	// ------------------------------------------------------------------------
	MainViewer::MainViewer(QWidget *parent) : NQVTKWidget(parent), field(0)
	{
		NQVTK::LayeredRaycastingRenderer *renderer = 
			new NQVTK::LayeredRaycastingRenderer();

		style = new SimiBlobStyle();
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
	void MainViewer::SetField(Field *field)
	{
		this->field = field;
		GetRenderer()->SetScene(field->GetScene());
		style->SetField(field);
	}
}
