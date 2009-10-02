#include "VectorNPRStyle.h"

#include "GLBlaat/GLProgram.h"

#include "Shaders.h"

#include <QApplication>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Data/VectorField.h"

namespace VFE
{
	// ------------------------------------------------------------------------
	// Helper function
	std::string LoadShader(const std::string &filename)
	{
		// TODO: embed shader sources in the app for final version
		std::string apppath = QApplication::applicationDirPath().toStdString();
		// Check up to 2 levels up in the directory hierarchy
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
	VectorNPRStyle::VectorNPRStyle() : field(0)
	{
		// TODO: adjust this based on the current number of features
		SetOption("VFE_FEATURECOUNT", 4);
		SetOption("NQVTK_BITMASK_BITS", 8);
	}

	// ------------------------------------------------------------------------
	VectorNPRStyle::~VectorNPRStyle()
	{
	}

	// ------------------------------------------------------------------------
	GLProgram *VectorNPRStyle::CreateScribe()
	{
		SetScribeSource(
			LoadShader("VFENPRScribeVS.txt"), 
			LoadShader("VFENPRScribeFS.txt"));
		return Superclass::CreateScribe();
	}

	// ------------------------------------------------------------------------
	GLProgram *VectorNPRStyle::CreateRaycaster()
	{
		SetRaycasterSource(
			LoadShader("VFENPRRaycasterVS.txt"), 
			LoadShader("VFENPRRaycasterFS.txt"));
		return Superclass::CreateRaycaster();
	}

	// ------------------------------------------------------------------------
	GLProgram *VectorNPRStyle::CreatePainter()
	{
		SetPainterSource(
			LoadShader("VFENPRPainterVS.txt"), 
			LoadShader("VFENPRPainterFS.txt"));
		return Superclass::CreatePainter();
	}

	// ------------------------------------------------------------------------
	void VectorNPRStyle::UpdatePainterParameters(GLProgram *painter)
	{
		Superclass::UpdatePainterParameters(painter);

		if (field) field->SetupFeatures(painter);
	}

	// ------------------------------------------------------------------------
	void VectorNPRStyle::SetField(const VectorField *field)
	{
		this->field = field;
	}
}
