#include "PropertySlider.h"
#include "PropertySlider.moc"

namespace MFE
{
	// ------------------------------------------------------------------------
	PropertySlider::PropertySlider(int baseIndex, int numSignals, 
		QWidget *parent) : QSlider(Qt::Horizontal, parent), 
		baseIndex(baseIndex), numSignals(numSignals)
	{
		connect(this, SIGNAL(valueChanged(int)), 
			this, SLOT(OnValueChanged(int)));
	}

	// ------------------------------------------------------------------------
	void PropertySlider::OnValueChanged(int value)
	{
		float v = static_cast<float>(value) / 100.0;
		for (int i = 0; i < numSignals; ++i)
		{
			emit ValueChanged(v, baseIndex + i);
		}
		emit ValueTextChanged(QString("%1").arg(v, 0, 'f', 2));
	}
}
