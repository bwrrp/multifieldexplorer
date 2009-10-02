#include "MainWindow.h"
#include "MainWindow.moc"

#include "Data/Feature.h"
#include "Data/VectorField.h"

#include <NQVTK/Rendering/Renderer.h>
#include <NQVTK/Rendering/Scene.h>

#include <NQVTK/Renderables/Renderable.h>

#include <QApplication>
#include <QFileDialog>

namespace VFE
{
	// ------------------------------------------------------------------------
	MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), field(0)
	{
		ui.setupUi(this);

		rcdialog = new RenderControls(ui.mainViewer, this);

		connect(ui.sliceViewer, SIGNAL(CursorPosChanged(NQVTK::Vector3)), 
			ui.featureList, SLOT(SetCursorPos(NQVTK::Vector3)));
		connect(ui.sliceViewer, SIGNAL(PointSelected(NQVTK::Vector3)), 
			ui.featureList, SLOT(SetSelectedPos(NQVTK::Vector3)));

		connect(ui.featureList, SIGNAL(Updated()), 
			this, SLOT(RedrawViewers()));

		connect(ui.featureList, SIGNAL(FeatureSelected(Feature*)), 
			ui.featureEditor, SLOT(SetFeature(Feature*)));
		connect(ui.featureEditor, SIGNAL(Updated()), 
			this, SLOT(RedrawViewers()));
	}

	// ------------------------------------------------------------------------
	MainWindow::~MainWindow()
	{
		delete rcdialog;
	}

	// ------------------------------------------------------------------------
	void MainWindow::LoadField(const QString &filename)
	{
		VectorField *newField = VectorField::Load(filename);
		if (newField)
		{
			if (field)
			{
				ui.mainViewer->GetRenderer()->SetScene(0);
				ui.sliceViewer->GetRenderer()->SetScene(0);
				delete field;
				field = 0;
			}
			field = newField;

			ui.mainViewer->SetField(field);
			ui.sliceViewer->GetRenderer()->SetScene(field->GetScene());
			ui.featureList->SetField(field);
			RedrawViewers();
		}
	}

	// ------------------------------------------------------------------------
	void MainWindow::RedrawViewers()
	{
		ui.mainViewer->updateGL();
		ui.sliceViewer->updateGL();
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionLoad_triggered()
	{
		// Show open dialog
		QString filename = QFileDialog::getOpenFileName(
			this, "Load vector field", QString(), 
			"Volume data (*.vti *.mha *.mhd);;"
			"VTK volumes (*.vti);;"
			"MetaImage files (*.mha *.mhd)");
		// This returns a null string when cancelled
		if (!filename.isNull())
		{
			LoadField(filename);
		}
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionQuit_triggered()
	{
		qApp->quit();
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionRenderingOptions_triggered()
	{
		rcdialog->show();
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionReloadShaders_triggered()
	{
		ui.mainViewer->Initialize();
		ui.sliceViewer->Initialize();
		RedrawViewers();
	}
}
