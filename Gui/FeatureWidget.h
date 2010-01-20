#pragma once

#include <QWidget>

#include "ui_FeatureWidget.h"

class QLabel;

namespace MFE
{
	class Field;
	class Feature;

	class FeatureWidget : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureWidget(QWidget *parent = 0);

	public slots:
		void SetField(Field *field);
		void SetFeature(Feature *feature);
		void UpdateView(Feature *updatedFeature = 0);
		void InitializeFeedbackViewer();
		void RedrawFeedbackViewer();

	signals:
		void Updated();

	protected:
		Ui::FeatureWidget ui;
		Feature *feature;

		void CreatePropertyWidgets(Field *field);
		void UpdatePropertyWidgets();

	private slots:
		void on_enabled_toggled(bool checked);
		void on_startThreshold_valueChanged(int value);
		void on_endThreshold_valueChanged(int value);
		void on_power_valueChanged(int value);
		void on_stretch_valueChanged(int value);
		void on_feedbackView_Updated();
	};
}
