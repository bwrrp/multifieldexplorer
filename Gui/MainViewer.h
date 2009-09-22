#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

#include <NQVTK/Math/Vector3.h>

namespace VFE
{
	class VectorNPRStyle;

	class MainViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		MainViewer(QWidget *parent = 0);
		virtual ~MainViewer();

	public slots:
		void SetCursorPos(NQVTK::Vector3);

	private:
		VectorNPRStyle *style;
	};
}
