#pragma once

#include <QMainWindow>
#include <QString>

#include "ui_MainWindow.h"

#include "RenderControls.h"

namespace MFE
{
	class Field;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT;

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

		void LoadField(const QString &filename);

	public slots:
		void RedrawViewers();

	protected:
		void SaveScreenshot(NQVTKWidget *viewer, const QString &filename, 
			bool invertAlpha = false);

		Ui::MainWindow ui;

		RenderControls *rcdialog;

		Field *field;

	private slots:
		void on_actionLoad_triggered();
		void on_actionQuit_triggered();
		void on_actionRenderingOptions_triggered();
		// TODO: remove this and turn shaders into resource
		void on_actionReloadShaders_triggered();
		void on_actionStartBenchmark_triggered();
		void on_actionScreenshot_triggered();
		void on_mainViewer_fpsChanged(int fps);
	};
}
