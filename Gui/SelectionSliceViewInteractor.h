#pragma once

#include <QObject>

#include <NQVTK/Interactors/SliceViewInteractor.h>

#include <NQVTK/Math/Vector3.h>

namespace MFE
{
	// Only classes derived from QObject can emit signals, 
	// therefore we use a separate messenger object.
	class SelectionMessenger : public QObject
	{
		Q_OBJECT;

	public:
		void EmitCursorPosChanged(const NQVTK::Vector3 &pos);
		void EmitPointSelected(const NQVTK::Vector3 &pos);

	signals:
		void CursorPosChanged(NQVTK::Vector3 pos);
		void PointSelected(NQVTK::Vector3 pos);
	};

	class SelectionSliceViewInteractor : public NQVTK::SliceViewInteractor
	{
	public:
		typedef SliceViewInteractor Superclass;

		SelectionSliceViewInteractor(NQVTK::SliceRenderer *renderer);
		virtual ~SelectionSliceViewInteractor();

		SelectionMessenger *GetMessenger() { return messenger; }

		virtual bool MouseMoveEvent(NQVTK::MouseEvent event);
		virtual bool MouseReleaseEvent(NQVTK::MouseEvent event);

	protected:
		NQVTK::Vector3 Get3DPos(const NQVTK::MouseEvent &event);

	private:
		SelectionMessenger *messenger;
	};
}
