#pragma once

#include <QWidget>

#include "ui_FeatureList.h"

namespace VFE
{
	class Feature;
	class VectorField;

	class FeatureList : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureList(QWidget *parent = 0);

		void SetField(VectorField *field);

	signals:
		void FeatureSelected(Feature *feature);

	protected:
		Ui::FeatureList ui;
		VectorField *field;

	private slots:
		void on_featureList_currentRowChanged(int row);
	};
}
