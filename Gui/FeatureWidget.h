#pragma once

#include <QWidget>

namespace VFE
{
	class FeatureWidget : public QWidget
	{
		Q_OBJECT;

	public:
		FeatureWidget(QWidget *parent = 0);
	};
}
