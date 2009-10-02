#include "FeatureList.h"
#include "FeatureList.moc"

namespace VFE
{
	// ------------------------------------------------------------------------
	FeatureList::FeatureList(QWidget *parent) : QWidget(parent)
	{
		ui.setupUi(this);
	}

	// ------------------------------------------------------------------------
	void FeatureList::SetField(VectorField *field)
	{
		this->field = field;
		// TODO: update list of features
	}
}
