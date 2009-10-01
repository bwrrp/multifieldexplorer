#include <GLBlaat/GL.h>
#include "MainViewer.h"
#include "MainViewer.moc"

#include <NQVTK/Interactors/ArcballCameraInteractor.h>

#include <NQVTK/Rendering/ArcballCamera.h>
#include <NQVTK/Rendering/LayeredRaycastingRenderer.h>

#include "Data/VectorField.h"

#include "Rendering/VectorNPRStyle.h"

namespace VFE
{
	// ------------------------------------------------------------------------
	MainViewer::MainViewer(QWidget *parent) : NQVTKWidget(parent), field(0)
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
	void MainViewer::SetField(VectorField *field)
	{
		this->field = field;
		GetRenderer()->SetScene(field->GetScene());
		style->SetField(field);
	}

	// ------------------------------------------------------------------------
	void MainViewer::SetCursorPos(NQVTK::Vector3 pos)
	{
		if (field)
		{
			field->SetCursorPos(pos);
			updateGL();
		}
	}

	// ------------------------------------------------------------------------
	void MainViewer::SetSelectedPos(NQVTK::Vector3 pos)
	{
		if (field)
		{
			field->SetSelectedPos(pos);
			updateGL();
		}
	}
}
