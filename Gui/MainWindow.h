#pragma once

#include <QMainWindow>
#include <QString>

#include "ui_MainWindow.h"

namespace VFE
{
	class VectorField;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT;

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

		void LoadField(const QString &filename);

	protected:
		Ui::MainWindow ui;

		VectorField *field;

	private slots:
		void on_actionLoad_triggered();
		void on_actionQuit_triggered();

		// TODO: remove this and turn shaders into resource
		void on_actionReloadShaders_triggered();
	};
}
