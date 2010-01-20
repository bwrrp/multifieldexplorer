#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

namespace MFE
{
	class Field;
	class Feature;

	class FeedbackViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		FeedbackViewer(QWidget *parent = 0);
		virtual ~FeedbackViewer();

	public slots:
		void SetField(Field *field);
		void SetFeature(Feature *feature);

	signals:
		void Updated();
	};
}
