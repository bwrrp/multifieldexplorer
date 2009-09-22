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
		GLProgram *scribe = GLProgram::New();
		// Scribe vertex shaders
		bool res = scribe->AddVertexShader(
			AddShaderDefines(LoadShader("VFENPRScribeVS.txt")));
		// Scribe fragment shaders
		if (res) res = scribe->AddFragmentShader(
			AddShaderDefines(LoadShader("VFENPRScribeFS.txt")));
		if (res) res = scribe->AddFragmentShader(
			AddShaderDefines(Shaders::LibUtility));
		if (res) res = scribe->Link();
		std::cout << scribe->GetInfoLogs() << std::endl;
		if (!res)
		{
			delete scribe;
			return 0;
		}
		return scribe;
	}

	// ------------------------------------------------------------------------
	GLProgram *VectorNPRStyle::CreateRaycaster()
	{
		GLProgram *raycaster = GLProgram::New();
		// raycaster vertex shaders
		bool res = raycaster->AddVertexShader(
			AddShaderDefines(LoadShader("VFENPRRaycasterVS.txt")));
		// raycaster fragment shaders
		if (res) res = raycaster->AddFragmentShader(
			AddShaderDefines(LoadShader("VFENPRRaycasterFS.txt")));
		if (res) res = raycaster->AddFragmentShader(
			AddShaderDefines(Shaders::LibUtility));
		if (res) res = raycaster->Link();
		std::cout << raycaster->GetInfoLogs() << std::endl;
		if (!res)
		{
			delete raycaster;
			return 0;
		}
		return raycaster;
	}

	// ------------------------------------------------------------------------
	GLProgram *VectorNPRStyle::CreatePainter()
	{
		GLProgram *painter = GLProgram::New();
		// painter vertex shaders
		bool res = painter->AddVertexShader(
			AddShaderDefines(LoadShader("VFENPRPainterVS.txt")));
		// painter fragment shaders
		if (res) res = painter->AddFragmentShader(
			AddShaderDefines(LoadShader("VFENPRPainterFS.txt")));
		if (res) res = painter->AddFragmentShader(
			AddShaderDefines(Shaders::LibUtility));
		if (res) res = painter->Link();
		std::cout << painter->GetInfoLogs() << std::endl;
		if (!res)
		{
			delete painter;
			return 0;
		}
		return painter;
	}
}
