#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

namespace VFE
{
	class MainViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		MainViewer(QWidget *parent = 0);
		virtual ~MainViewer();
	};
}
