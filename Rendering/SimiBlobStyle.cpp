#include "SimiBlobStyle.h"

#include "GLBlaat/GLProgram.h"

#include "Shaders.h"

#include <QApplication>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Data/Field.h"

namespace MFE
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
	SimiBlobStyle::SimiBlobStyle() : field(0)
	{
		// TODO: adjust this based on the current number of features
		SetOption("MFE_FEATURECOUNT", 6);
		SetOption("NQVTK_BITMASK_BITS", 8);
	}

	// ------------------------------------------------------------------------
	SimiBlobStyle::~SimiBlobStyle()
	{
	}

	// ------------------------------------------------------------------------
	GLProgram *SimiBlobStyle::CreateScribe()
	{
		SetScribeSource(
			LoadShader("SimiBlobScribeVS.txt"), 
			LoadShader("SimiBlobScribeFS.txt"));
		return Superclass::CreateScribe();
	}

	// ------------------------------------------------------------------------
	GLProgram *SimiBlobStyle::CreateRaycaster()
	{
		SetRaycasterSource(
			LoadShader("SimiBlobRaycasterVS.txt"), 
			LoadShader("SimiBlobRaycasterFS.txt"));
		return Superclass::CreateRaycaster();
	}

	// ------------------------------------------------------------------------
	GLProgram *SimiBlobStyle::CreatePainter()
	{
		SetPainterSource(
			LoadShader("SimiBlobPainterVS.txt"), 
			LoadShader("SimiBlobPainterFS.txt"));
		return Superclass::CreatePainter();
	}

	// ------------------------------------------------------------------------
	void SimiBlobStyle::UpdatePainterParameters(GLProgram *painter)
	{
		Superclass::UpdatePainterParameters(painter);

		if (field) field->SetupFeatures(painter);
	}

	// ------------------------------------------------------------------------
	void SimiBlobStyle::SetField(const Field *field)
	{
		this->field = field;
	}
}
