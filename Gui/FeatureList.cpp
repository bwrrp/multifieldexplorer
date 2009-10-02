#include "FeatureList.h"
#include "FeatureList.moc"

#include "Data/Feature.h"
#include "Data/VectorField.h"

#include <QString>

#include <cassert>

namespace VFE
{
	// ------------------------------------------------------------------------
	FeatureList::FeatureList(QWidget *parent) : QWidget(parent)
	{
		ui.setupUi(this);
		SetField(0);
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetField(VectorField *field)
	{
		this->field = field;
		if (field)
		{
			int numFeatures = field->GetNumberOfFeatures();
			assert(numFeatures > 0);
			ui.featureList->addItem("Cursor");
			for (int i = 1; i < numFeatures; ++i)
			{
				Feature *f = field->GetFeature(i);
				ui.featureList->addItem(QString("<%1, %2, %3>")
					.arg(f->examplePos.x)
					.arg(f->examplePos.y)
					.arg(f->examplePos.z));
			}
		}
		else
		{
			ui.featureList->clear();
			emit FeatureSelected(0);
		}
	}

	// ------------------------------------------------------------------------
	void FeatureList::on_featureList_currentRowChanged(int row)
	{
		Feature *f = 0;
		if (field) f = field->GetFeature(row);
		
		emit FeatureSelected(f);
	}
}
