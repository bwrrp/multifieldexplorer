#include "RenderControls.h"
#include "RenderControls.moc"

#include <NQVTK/Rendering/LayeredRaycastingRenderer.h>

#include "Rendering/VectorNPRStyle.h"

#include "mainViewer.h"

namespace VFE
{
	// ------------------------------------------------------------------------
	RenderControls::RenderControls(MainViewer *mainViewer, QWidget *parent) 
		: QWidget(parent, Qt::Tool), mainViewer(mainViewer)
	{
		ui.setupUi(this);
	}

	// ------------------------------------------------------------------------
	void RenderControls::on_maxLayers_valueChanged(int value)
	{
		NQVTK::LayeredRaycastingRenderer *renderer = 
			dynamic_cast<NQVTK::LayeredRaycastingRenderer*>(
			mainViewer->GetRenderer());
		renderer->maxLayers = value;
		ui.maxLayersReadout->setText(QString("%1").arg(value));
		mainViewer->updateGL();
	}

	// ------------------------------------------------------------------------
	void RenderControls::on_skipLayers_valueChanged(int value)
	{
		NQVTK::LayeredRaycastingRenderer *renderer = 
			dynamic_cast<NQVTK::LayeredRaycastingRenderer*>(
			mainViewer->GetRenderer());
		renderer->skipLayers = value;
		ui.skipLayersReadout->setText(QString("%1").arg(value));
		mainViewer->updateGL();
	}

	// ------------------------------------------------------------------------
	void RenderControls::on_stepSize_valueChanged(int value)
	{
		VectorNPRStyle *style = mainViewer->GetStyle();
		float v = static_cast<float>(value) / 100.0;
		ui.stepSizeReadout->setText(QString("%1").arg(v, 0, 'f', 3));
		style->stepSize = v;
		mainViewer->updateGL();
	}

	// ------------------------------------------------------------------------
	void RenderControls::on_kernelSize_valueChanged(int value)
	{
		VectorNPRStyle *style = mainViewer->GetStyle();
		float v = static_cast<float>(value) / 100.0;
		ui.kernelSizeReadout->setText(QString("%1").arg(v, 0, 'f', 3));
		style->kernelSize = v;
		mainViewer->updateGL();
	}
}
