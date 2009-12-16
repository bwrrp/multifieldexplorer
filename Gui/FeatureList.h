#pragma once

#include <QWidget>

#include "ui_FeatureList.h"

#include <NQVTK/Math/Vector3.h>

namespace MFE
{
	class Feature;
	class Field;

	class FeatureList : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureList(QWidget *parent = 0);

		void SetField(Field *field);

	public slots:
		void SetCursorPos(NQVTK::Vector3);
		void SetSelectedPos(NQVTK::Vector3);

	signals:
		void FeatureSelected(Feature *feature);
		void FeatureUpdated(Feature *feature);
		void Updated();

	protected:
		Ui::FeatureList ui;
		Field *field;

	private slots:
		void UpdateFeature(int num);
		void on_featureList_currentRowChanged(int row);
		void on_addFeature_clicked();
		void on_removeFeature_clicked();
	};
}
