#pragma once

#include <QWidget>

#include "ui_FeatureWidget.h"

namespace VFE
{
	class Feature;

	class FeatureWidget : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureWidget(QWidget *parent = 0);

	public slots:
		void SetFeature(Feature *feature);
		void UpdateView(Feature *updatedFeature = 0);

	signals:
		void Updated();

	protected:
		Ui::FeatureWidget ui;
		Feature *feature;

	private slots:
		void on_enabled_toggled(bool checked);
		void on_startThreshold_valueChanged(int value);
		void on_endThreshold_valueChanged(int value);
	};
}
