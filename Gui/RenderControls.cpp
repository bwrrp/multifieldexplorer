#include "RenderControls.h"
#include "RenderControls.moc"

#include <NQVTK/Rendering/LayeredRaycastingRenderer.h>

#include "Rendering/SliceRenderer.h"
#include "Rendering/SimiBlobStyle.h"

#include "MainViewer.h"
#include "SliceViewer.h"

namespace MFE
{
	// ------------------------------------------------------------------------
	RenderControls::RenderControls(MainViewer *mainViewer, 
		SliceViewer *sliceViewer, QWidget *parent) : QWidget(parent, Qt::Tool), 
		mainViewer(mainViewer), sliceViewer(sliceViewer)
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
		SimiBlobStyle *style = mainViewer->GetStyle();
		float v = static_cast<float>(value) / 100.0;
		ui.stepSizeReadout->setText(QString("%1").arg(v, 0, 'f', 2));
		style->stepSize = v;
		mainViewer->updateGL();
	}

	// ------------------------------------------------------------------------
	void RenderControls::on_kernelSize_valueChanged(int value)
	{
		SimiBlobStyle *style = mainViewer->GetStyle();
		float v = static_cast<float>(value) / 100.0;
		ui.kernelSizeReadout->setText(QString("%1").arg(v, 0, 'f', 2));
		style->kernelSize = v;		
		mainViewer->updateGL();
		SliceRenderer *renderer = dynamic_cast<SliceRenderer*>(
			sliceViewer->GetRenderer());
		if (renderer)
		{
			renderer->kernelSize = v;
			sliceViewer->updateGL();
		}
	}
}
