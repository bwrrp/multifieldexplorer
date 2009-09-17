#include "GLBlaat/GL.h"

#include "MainWindow.h"
#include "MainWindow.moc"

#include <QApplication>

#include <NQVTK/Rendering/SimpleRenderer.h>

namespace VFE
{
	// ------------------------------------------------------------------------
	MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
	{
		ui.setupUi(this);

		ui.mainViewer->SetRenderer(new NQVTK::SimpleRenderer());
		ui.sliceViewer->SetRenderer(new NQVTK::SimpleRenderer());
	}

	// ------------------------------------------------------------------------
	MainWindow::~MainWindow()
	{
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionLoad_triggered()
	{
		// TODO: show open dialog, load new dataset
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionQuit_triggered()
	{
		qApp->quit();
	}
}
