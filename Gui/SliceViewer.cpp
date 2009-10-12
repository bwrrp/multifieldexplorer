#include <GLBlaat/GL.h>
#include "SliceViewer.h"
#include "SliceViewer.moc"

#include "SelectionSliceViewInteractor.h"

#include <GLBlaat/GLProgram.h>

#include <NQVTK/Math/Vector3.h>

#include <NQVTK/Rendering/Camera.h>
#include <NQVTK/Rendering/SliceRenderer.h>

#include <QApplication>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace VFE
{
	// ------------------------------------------------------------------------
	// Helper function
	static std::string LoadShader(const std::string &filename)
	{
		// TODO: embed shader sources in the app for final version
		std::string apppath = QApplication::applicationDirPath().toStdString();
		// Check up to 4 levels up in the directory hierarchy
		std::string searchpath = "/Shaders/" + filename;
		std::ifstream infile((apppath + searchpath).c_str());
		for (int i = 0; i < 4 && !infile; ++i)
		{
			searchpath =  "/.." + searchpath;
			infile.close();
			infile.clear();
			infile.open((apppath + searchpath).c_str());
		}
		std::ostringstream contents;
		if (infile.good())
		{
			contents << infile.rdbuf();
		}
		else
		{
			std::cerr << "Error loading shader!" << std::endl;
		}
		return contents.str();
	}

	// ------------------------------------------------------------------------
	static GLProgram *CreateSliceViewerShader()
	{
		GLProgram *shader = GLProgram::New();
		if (!shader) return false;
		bool ok = true;
		if (ok) ok = shader->AddVertexShader(
			LoadShader("SliceViewerVS.txt"));
		// TODO: we need the actual object transform here
		if (ok) ok = shader->AddFragmentShader(
			LoadShader("SliceViewerFS.txt"));
		if (ok) ok = shader->Link();
		if (!ok) 
		{
			delete shader;
			return 0;
		}
		return shader;
	}

	// ------------------------------------------------------------------------
	SliceViewer::SliceViewer(QWidget *parent) : NQVTKWidget(parent)
	{
		NQVTK::SliceRenderer *renderer = new NQVTK::SliceRenderer();
		renderer->SetPlane(
			NQVTK::Vector3(-10.0, -10.0, 5.0), 
			NQVTK::Vector3(20.0, 0.0, 0.0), 
			NQVTK::Vector3(0.0, 20.0, 0.0));
		SetRenderer(renderer);
		SelectionSliceViewInteractor *interactor = 
            new SelectionSliceViewInteractor(renderer);
		SetInteractor(interactor);
		// Display crosshair
		toggleCrosshair(true);
		connect(this, SIGNAL(cursorPosChanged(double, double)), 
			this, SLOT(setCrosshairPos(double, double)));
		// Expose interactor events
		connect(interactor->GetMessenger(), 
			SIGNAL(CursorPosChanged(NQVTK::Vector3)), 
			this, SIGNAL(CursorPosChanged(NQVTK::Vector3)));
		connect(interactor->GetMessenger(), 
			SIGNAL(PointSelected(NQVTK::Vector3)), 
			this, SIGNAL(PointSelected(NQVTK::Vector3)));
	}

	// ------------------------------------------------------------------------
	SliceViewer::~SliceViewer()
	{
	}

    // ------------------------------------------------------------------------
	void SliceViewer::initializeGL()
	{
		Superclass::initializeGL();

		NQVTK::SliceRenderer *renderer = 
			dynamic_cast<NQVTK::SliceRenderer*>(GetRenderer());

		GLProgram *shader = CreateSliceViewerShader();
		if (!shader)
		{
			qDebug("Could not create slice viewer shader!");
		}
		GLProgram *oldShader = renderer->SetShader(shader);
		delete oldShader;
	}
}
