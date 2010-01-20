#pragma once

#include <QObject>

#include <NQVTK/Interactors/Interactor.h>

namespace MFE
{
	// Only classes derived from QObject can emit signals, 
	// therefore we use a separate messenger object.
	class MaskMessenger : public QObject
	{
		Q_OBJECT;

	public:
		void EmitUpdated();

	signals:
		void Updated();
	};

	class Feature;

	class FeedbackMaskInteractor : public NQVTK::Interactor
	{
	public:
		FeedbackMaskInteractor();
		virtual ~FeedbackMaskInteractor();

		void SetFeature(Feature *feature);

		MaskMessenger *GetMessenger() { return messenger; }

		virtual bool MouseMoveEvent(NQVTK::MouseEvent event);
		virtual bool MouseReleaseEvent(NQVTK::MouseEvent event);

	private:
		MaskMessenger *messenger;
		Feature *currentFeature;

		int lastPid;
	};
}
