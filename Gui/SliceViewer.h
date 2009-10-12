#pragma once

#include <NQVTKGui/Viewports/NQVTKWidget.h>

#include <NQVTK/Math/Vector3.h>

namespace VFE
{
	class VectorField;
	class SliceRenderer;

	class SliceViewer : public NQVTKWidget
	{
		Q_OBJECT;

	public:
		typedef NQVTKWidget Superclass;

		SliceViewer(QWidget *parent = 0);
		virtual ~SliceViewer();

		void SetField(VectorField *field);

	signals:
		void CursorPosChanged(NQVTK::Vector3 pos);
		void PointSelected(NQVTK::Vector3 pos);
	};
}
