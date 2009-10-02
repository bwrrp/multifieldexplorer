#pragma once

#include <QWidget>

#include "ui_FeatureList.h"

namespace VFE
{
	class VectorField;

	class FeatureList : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureList(QWidget *parent = 0);

		void SetField(VectorField *field);

	protected:
		Ui::FeatureList ui;
		VectorField *field;
	};
}
