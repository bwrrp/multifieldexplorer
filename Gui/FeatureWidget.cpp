#include "FeatureWidget.h"
#include "FeatureWidget.moc"

#include "Data/Feature.h"

namespace VFE
{
	// ------------------------------------------------------------------------
	FeatureWidget::FeatureWidget(QWidget *parent) : QWidget(parent)
	{
		ui.setupUi(this);
		// TODO: create property widgets
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::SetFeature(Feature *feature)
	{
		this->feature = feature;
		if (feature)
		{
			// Update widgets
			ui.enabled->setChecked(feature->enabled);
			ui.startThreshold->setValue(feature->startThreshold * 100);
			ui.endThreshold->setValue(feature->endThreshold * 100);
			// TODO: update state of property widgets
			setEnabled(true);
		}
		else
		{
			setEnabled(false);
		}
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
			feature->startThreshold = static_cast<float>(value) / 100.0;
			emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_endThreshold_valueChanged(int value)
	{
		if (feature)
		{
			feature->endThreshold = static_cast<float>(value) / 100.0;
			emit Updated();
		}
	}
}
