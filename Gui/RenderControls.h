#pragma once

#include <QWidget>

#include "ui_RenderControls.h"

namespace VFE
{
	class MainViewer;

	class RenderControls : public QWidget
	{
		Q_OBJECT;

	public:
		RenderControls(MainViewer *mainViewer, QWidget *parent = 0);

	protected:
		Ui::RenderControls ui;

		MainViewer *mainViewer;

	private slots:
		void on_maxLayers_valueChanged(int value);
		void on_skipLayers_valueChanged(int value);
		void on_stepSize_valueChanged(int value);
		void on_kernelSize_valueChanged(int value);
	};
}
