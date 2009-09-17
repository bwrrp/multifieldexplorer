#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

namespace VFE
{
	class SliceViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		SliceViewer(QWidget *parent = 0);
		virtual ~SliceViewer();
	};
}
