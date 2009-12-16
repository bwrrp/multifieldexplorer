#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

#include <NQVTK/Math/Vector3.h>

namespace MFE
{
	class Field;
	class SimiBlobStyle;

	class MainViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		MainViewer(QWidget *parent = 0);
		virtual ~MainViewer();

		void SetField(Field *field);

		SimiBlobStyle *GetStyle() { return style; }

	private:
		Field *field;
		SimiBlobStyle *style;
	};
}
