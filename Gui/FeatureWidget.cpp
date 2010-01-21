#include "FeatureWidget.h"
#include "FeatureWidget.moc"

#include "Data/Field.h"
#include "Data/Feature.h"

#include <QGridLayout>
#include <QLabel>

namespace MFE
{
	// ------------------------------------------------------------------------
	FeatureWidget::FeatureWidget(QWidget *parent) : QWidget(parent)
	{
		ui.setupUi(this);
		SetFeature(0);
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::SetField(Field *field)
	{
		// Unset the current feature
		SetFeature(0);

		// Create property labels for the new field
		CreatePropertyWidgets(field);

		// Pass to the feedback view to setup the property transform
		ui.feedbackView->SetField(field);
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::SetFeature(Feature *feature)
	{
		if (this->feature == feature) return;

		this->feature = feature;
		ui.feedbackView->SetFeature(feature);

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
				ui.biasRelativeToMean->setChecked(feature->biasRelativeToMean);
				if (feature->biasRelativeToMean)
				{
					ui.backgroundPosReadout->setText("Mean value");
				}
				else
				{
					ui.backgroundPosReadout->setText(QString("<%1, %2, %3>")
						.arg(feature->backgroundPos.x)
						.arg(feature->backgroundPos.y)
						.arg(feature->backgroundPos.z));
				}
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
				ui.examplePosReadout->setText("(no feature selected)");
				setEnabled(false);
			}
		}
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::InitializeFeedbackViewer()
	{
		ui.feedbackView->Initialize();
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::RedrawFeedbackViewer()
	{
		ui.feedbackView->updateGL();
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::CreatePropertyWidgets(Field *field)
	{
		// TODO: create labels for the properties underneath the feedback view
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::UpdatePropertyWidgets()
	{
		ui.feedbackView->updateGL();
	}

	// ------------------------------------------------------------------------
	void FeatureWidget::on_biasRelativeToMean_toggled(bool checked)
	{
		if (feature)
		{
			feature->biasRelativeToMean = checked;
			UpdateView(feature);
			emit Updated();
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
	void FeatureWidget::on_feedbackView_Updated()
	{
		// The mask was changed
		if (feature != 0 && feature->enabled) emit Updated();
	}
}
