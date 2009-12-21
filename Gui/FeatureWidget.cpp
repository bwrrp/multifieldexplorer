#include "FeatureWidget.h"
#include "FeatureWidget.moc"

#include "Data/Feature.h"

#include <QGridLayout>
#include <QLabel>

#include "PropertySlider.h"

namespace MFE
{
	// ------------------------------------------------------------------------
	FeatureWidget::FeatureWidget(QWidget *parent) : QWidget(parent)
	{
		ui.setupUi(this);
		CreatePropertyWidgets();
		SetFeature(0);
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::SetFeature(Feature *feature)
	{
		if (this->feature == feature) return;

		this->feature = feature;
		UpdateView();
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::UpdateView(Feature *updatedFeature)
	{
		if (updatedFeature == 0 || updatedFeature == feature)
		{
			if (feature)
			{
				// Update widgets
				ui.enabled->setChecked(feature->enabled);
				ui.startThreshold->setValue(
					0.5 + 100.0 * feature->startThreshold);
				ui.endThreshold->setValue(
					0.5 + 100.0 * feature->endThreshold);
				ui.examplePosReadout->setText(QString("<%1, %2, %3>")
					.arg(feature->examplePos.x)
					.arg(feature->examplePos.y)
					.arg(feature->examplePos.z));
				ui.power->setValue(
					0.5 + 100.0 * feature->power);
				ui.stretch->setValue(
					0.5 + 100.0 * feature->stretch);
				UpdatePropertyWidgets();
				setEnabled(true);
			}
			else
			{
				setEnabled(false);
			}
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::CreatePropertyWidgets()
	{
		/*
		// Create a dummy feature to get the properties
		Feature f;
		// Get layout
		QGridLayout *layout = dynamic_cast<QGridLayout*>(
			ui.propertiesGroup->layout());
		ui.propertyPlaceholder->hide();
		// Make widgets for the mask
		FeatureVector &fv = f.mask;
		int row = 0;
		int valueIndex = 0;
		for (std::vector<Property>::const_iterator it = fv.properties.begin();
			it != fv.properties.end(); ++it)
		{
			int size = it->GetSize();

			// Create widgets
			QString name(it->name.c_str());
			name[0] = name[0].toTitleCase();
			QLabel *label = new QLabel(name + ":", ui.propertiesGroup);
			layout->addWidget(label, row, 0);

			PropertySlider *slider = new PropertySlider(
				valueIndex, size, this);
			slider->setRange(0, 100);
			slider->setValue(0);
			QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Fixed);
			pol.setHorizontalStretch(1);
			slider->setSizePolicy(pol);
			layout->addWidget(slider, row, 1);
			propertySliders.push_back(slider);

			QLabel *readout = new QLabel("0.00", this);
			layout->addWidget(readout, row, 2);

			// Connect signals
			connect(slider, SIGNAL(ValueChanged(float, int)), 
				this, SLOT(PropertyValueChanged(float, int)));
			connect(slider, SIGNAL(ValueTextChanged(QString)), 
				readout, SLOT(setText(QString)));

			valueIndex += size;
			++row;
		}
		*/
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::UpdatePropertyWidgets()
	{
		/*
		if (!feature) return;
		FeatureVector &fv = feature->mask;

		int valueIndex = 0;
		for (unsigned int i = 0; i < propertySliders.size(); ++i)
		{
			PropertySlider *slider = propertySliders[i];
			int size = fv.properties[i].GetSize();
			slider->setValue(0.5 + 100.0 * fv.values[valueIndex]);
			valueIndex += size;
		}
		*/
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_enabled_toggled(bool checked)
	{
		if (feature)
		{
			feature->enabled = checked;
			emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_startThreshold_valueChanged(int value)
	{
		if (feature)
		{
			float v = static_cast<float>(value) / 100.0;
			feature->startThreshold = v;
			ui.startThresholdReadout->setText(QString("%1").arg(v, 0, 'f', 2));
			if (feature->enabled) emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_endThreshold_valueChanged(int value)
	{
		if (feature)
		{
			float v = static_cast<float>(value) / 100.0;
			feature->endThreshold = v;
			ui.endThresholdReadout->setText(QString("%1").arg(v, 0, 'f', 2));
			if (feature->enabled) emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_power_valueChanged(int value)
	{
		if (feature)
		{
			float v = static_cast<float>(value) / 100.0;
			feature->power = v;
			ui.powerReadout->setText(QString("%1").arg(v, 0, 'f', 2));
			if (feature->enabled) emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_stretch_valueChanged(int value)
	{
		if (feature)
		{
			float v = static_cast<float>(value) / 100.0;
			feature->stretch = v;
			ui.stretchReadout->setText(QString("%1").arg(v, 0, 'f', 2));
			if (feature->enabled) emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::PropertyValueChanged(float value, int index)
	{
		if (!feature) return;

		/*
		FeatureVector &fv = feature->mask;
		if (fv.values[index] != value)
		{
			fv.values[index] = value;
			if (feature->enabled) emit Updated();
		}
		*/
	}
}
