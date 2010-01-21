#include "FeatureList.h"
#include "FeatureList.moc"

#include "Data/Feature.h"
#include "Data/Field.h"

#include <QListWidgetItem>
#include <QString>

#include <cassert>

namespace MFE
{
	// ------------------------------------------------------------------------
	FeatureList::FeatureList(QWidget *parent) : QWidget(parent), field(0)
	{
		ui.setupUi(this);
		SetField(0);
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetField(Field *field)
	{
		if (this->field)
		{
			disconnect(this->field, SIGNAL(FeatureUpdated(int)), 
				this, SLOT(UpdateFeature(int)));
		}
		this->field = field;
		ui.featureList->clear();
		if (field)
		{
			int numFeatures = field->GetNumberOfFeatures();
			for (int i = 0; i < numFeatures; ++i)
			{
				Feature *f = field->GetFeature(i);
				if (i == 0)
				{
					ui.featureList->addItem("Cursor");
				}
				else
				{
					ui.featureList->addItem(QString("<%1, %2, %3>")
						.arg(f->examplePos.x)
						.arg(f->examplePos.y)
						.arg(f->examplePos.z));
				}
			}
			connect(this->field, SIGNAL(FeatureUpdated(int)), 
				this, SLOT(UpdateFeature(int)));
			ui.addFeature->setEnabled(true);
		}
		else
		{
			ui.addFeature->setEnabled(false);
			ui.removeFeature->setEnabled(false);
		}
		ui.featureList->clearSelection();
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetCursorPos(NQVTK::Vector3 pos)
	{
		if (field)
		{
			Feature *cursorFeature = field->GetFeature(0);
			if (cursorFeature)
			{
				cursorFeature->examplePos = pos;
				field->EmitFeatureUpdated(0);
			}
		}
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetSelectedPos(NQVTK::Vector3 pos)
	{
		if (field)
		{
			int num = ui.featureList->currentRow();
			Feature *selectedFeature = field->GetFeature(num);
			if (selectedFeature)
			{
				selectedFeature->examplePos = pos;
				field->EmitFeatureUpdated(num);
			}
		}
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetAlternatePos(NQVTK::Vector3 pos)
	{
		if (field)
		{
			int num = ui.featureList->currentRow();
			Feature *selectedFeature = field->GetFeature(num);
			if (selectedFeature)
			{
				selectedFeature->backgroundPos = pos;
				field->EmitFeatureUpdated(num);
			}
		}
	}

	// ------------------------------------------------------------------------
	void FeatureList::UpdateFeature(int num)
	{
		if (!field) return;
		Feature *f = field->GetFeature(num);
		if (f)
		{
			if (num > 0)
			{
				ui.featureList->item(num)->setText(QString("<%1, %2, %3>")
					.arg(f->examplePos.x)
					.arg(f->examplePos.y)
					.arg(f->examplePos.z));
			}
			if (f->enabled)
			{
				emit Updated();
			}
			// Emit signal so the editor can update if necessary
			// This is a bit of a hack, but ok...
			emit FeatureUpdated(f);
		}
	}

	// ------------------------------------------------------------------------
	void FeatureList::on_featureList_currentRowChanged(int row)
	{
		Feature *f = 0;
		if (field) f = field->GetFeature(row);

		// The cursor feature can never be deleted
		if (row > 0)
		{
			ui.removeFeature->setEnabled(true);
		}
		else
		{
			ui.removeFeature->setEnabled(false);
		}

		emit FeatureSelected(f);
	}

	// ------------------------------------------------------------------------
	void FeatureList::on_addFeature_clicked()
	{
		if (!field) return;

		// Reset selection
		emit FeatureSelected(0);

		ui.featureList->addItem("");
		field->AddFeature();
		field->EmitFeatureUpdated(field->GetNumberOfFeatures() - 1);
		ui.featureList->setCurrentRow(field->GetNumberOfFeatures() - 1);
		// TODO: max number of features should be variable
		ui.addFeature->setEnabled(field->GetNumberOfFeatures() < 6);
	}

	// ------------------------------------------------------------------------
	void FeatureList::on_removeFeature_clicked()
	{
		if (!field) return;

		// Remove from the list
		int num = ui.featureList->currentRow();
		delete ui.featureList->takeItem(num);
		// Was the feature enabled?
		bool enabled = field->GetFeature(num)->enabled;
		// Remove from the field
		field->RemoveFeature(num);

		// TODO: max number of features should be variable
		ui.addFeature->setEnabled(field->GetNumberOfFeatures() < 6);

		if (enabled) emit Updated();
	}
}
