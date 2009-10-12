#include "SliceRenderer.h"

#include <GLBlaat/GLProgram.h>

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
	SliceRenderer::SliceRenderer()
	{
	}

	// ------------------------------------------------------------------------
	bool SliceRenderer::Initialize()
	{
		bool initialized = Superclass::Initialize();

		GLProgram *shader = CreateSliceViewerShader();
		if (!shader)
		{
			qDebug("Could not create slice viewer shader!");
			initialized = false;
		}
		GLProgram *oldShader = SetShader(shader);
		delete oldShader;

		return initialized;
	}
}
