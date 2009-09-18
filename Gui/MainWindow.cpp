#include "GLBlaat/GL.h"

#include "MainWindow.h"
#include "MainWindow.moc"

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
	}

	// ------------------------------------------------------------------------
	MainWindow::~MainWindow()
	{
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

			ui.mainViewer->GetRenderer()->SetScene(field->GetScene());
			ui.sliceViewer->GetRenderer()->SetScene(field->GetScene());
		}
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
}
