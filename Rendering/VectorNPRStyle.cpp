#include "GLBlaat/GL.h"

#include "VectorNPRStyle.h"

#include "GLBlaat/GLProgram.h"

#include "Shaders.h"

#include <QApplication>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
		for (int i = 0; i < 2 && !infile; ++i)
		{
			searchpath =  "/.." + searchpath;
			infile.close();
			infile.clear();
			infile.open((apppath + searchpath).c_str());
		}
		std::cout << "Loading shader '" << apppath + searchpath << 
			"'..." << std::endl;
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
	VectorNPRStyle::VectorNPRStyle()
	{
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

		painter->SetUniform3f("cursorPos", 
			cursorPos.x, cursorPos.y, cursorPos.z);

		painter->SetUniform3f("selectedPos", 
			selectedPos.x, selectedPos.y, selectedPos.z);
	}
}
