#pragma once

#include <QMainWindow>

#include "ui_MainWindow.h"

namespace VFE
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT;

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:
		Ui::MainWindow ui;

	private slots:
		void on_actionLoad_triggered();
		void on_actionQuit_triggered();
	};
}
