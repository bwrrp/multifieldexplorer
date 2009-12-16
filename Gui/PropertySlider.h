#pragma once

#include <QSlider>
#include <QString>

namespace MFE
{
	class PropertySlider : public QSlider
	{
		Q_OBJECT;

	public:
		PropertySlider(int baseIndex, int numSignals = 1, QWidget *parent = 0);

	signals:
		void ValueChanged(float value, int index);
		void ValueTextChanged(QString value);

	protected:
		int baseIndex;
		int numSignals;

	private slots:
		void OnValueChanged(int value);
	};
}
