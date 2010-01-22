#include "MainWindow.h"
#include "MainWindow.moc"

#include "Data/Feature.h"
#include "Data/Field.h"

#include <NQVTK/Rendering/Renderer.h>
#include <NQVTK/Rendering/Scene.h>

#include <NQVTK/Renderables/Renderable.h>

#include <QApplication>
#include <QDateTime>
#include <QFileDialog>
#include <QImage>

namespace MFE
{
	// ------------------------------------------------------------------------
	MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), field(0)
	{
		ui.setupUi(this);

		rcdialog = new RenderControls(ui.mainViewer, ui.sliceViewer, this);

		connect(ui.sliceViewer, SIGNAL(CursorPosChanged(NQVTK::Vector3)), 
			ui.featureList, SLOT(SetCursorPos(NQVTK::Vector3)));
		connect(ui.sliceViewer, SIGNAL(PointSelected(NQVTK::Vector3)), 
			ui.featureList, SLOT(SetSelectedPos(NQVTK::Vector3)));
		connect(ui.sliceViewer, SIGNAL(AlternatePointSelected(NQVTK::Vector3)), 
			ui.featureList, SLOT(SetAlternatePos(NQVTK::Vector3)));

		connect(ui.featureList, SIGNAL(FeatureSelected(Feature*)), 
			ui.featureEditor, SLOT(SetFeature(Feature*)));
		connect(ui.featureList, SIGNAL(FeatureUpdated(Feature*)), 
			ui.featureEditor, SLOT(UpdateView(Feature*)));

		connect(ui.featureList, SIGNAL(Updated()), 
			this, SLOT(RedrawViewers()));
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
		Field *newField = Field::Load(filename);
		if (newField)
		{
			Field *oldField = field;
			field = newField;

			ui.mainViewer->SetField(field);
			ui.sliceViewer->SetField(field);
			ui.featureList->SetField(field);
			ui.featureEditor->SetField(field);

			// Shaders need to be rebuilt for the new field
			on_actionReloadShaders_triggered();

			delete oldField;
		}
	}

	// ------------------------------------------------------------------------
	void MainWindow::RedrawViewers()
	{
		ui.mainViewer->updateGL();
		ui.sliceViewer->updateGL();
		ui.featureEditor->RedrawFeedbackViewer();
	}

	// ------------------------------------------------------------------------
	void MainWindow::SaveScreenshot(NQVTKWidget *viewer, 
		const QString &filename, bool invertAlpha)
	{
		//QImage screenshot = viewer->grabFrameBuffer(true);
		QImage screenshot = viewer->GrabHighRes(6);
		if (invertAlpha)
		{
			// Fix alpha values
			screenshot.invertPixels(QImage::InvertRgba);
			screenshot.invertPixels(QImage::InvertRgb);
		}
		// Save it
		screenshot.save(filename, "PNG");
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionLoad_triggered()
	{
		// Show open dialog
		QString filename = QFileDialog::getOpenFileName(
			this, "Load field", QString(), 
			"Field description files (*.field)");
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
		ui.featureEditor->InitializeFeedbackViewer();
		RedrawViewers();
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionStartBenchmark_triggered()
	{
		ui.mainViewer->StartContinuousUpdate();
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_actionScreenshot_triggered()
	{
		QString now = QDateTime::currentDateTime().toString("yyMMdd-hhmmss");
		// Save screenshots of the main and slice viewers
		SaveScreenshot(ui.mainViewer, QString("MFE-%1-3D.png").arg(now), true);
		SaveScreenshot(ui.sliceViewer, QString("MFE-%1-Slice.png").arg(now));
	}

	// ------------------------------------------------------------------------
	void MainWindow::on_mainViewer_fpsChanged(int fps)
	{
		setWindowTitle(QString("Multi-Field Explorer - %1 fps").arg(fps));
	}
}
