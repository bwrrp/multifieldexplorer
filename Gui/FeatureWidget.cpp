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
	void FeatureWidget::UpdateView()
	{
		if (feature)
		{
			// Update widgets
			ui.enabled->setChecked(feature->enabled);
			ui.startThreshold->setValue(feature->startThreshold * 100);
			ui.endThreshold->setValue(feature->endThreshold * 100);
			ui.examplePosReadout->setText(QString("<%1, %2, %3>")
				.arg(feature->examplePos.x)
				.arg(feature->examplePos.y)
				.arg(feature->examplePos.z));
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
			float v = static_cast<float>(value) / 100.0;
			feature->startThreshold = v;
			ui.startThresholdReadout->setText(QString("%1").arg(v));
			emit Updated();
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_endThreshold_valueChanged(int value)
	{
		if (feature)
		{
			float v = static_cast<float>(value) / 100.0;
			feature->endThreshold = v;
			ui.endThresholdReadout->setText(QString("%1").arg(v));
			emit Updated();
		}
	}
}
