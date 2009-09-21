#include "GLBlaat/GL.h"

#include "VectorNPRStyle.h"

#include "GLBlaat/GLProgram.h"

#include "Shaders.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace VFE
{
	// ------------------------------------------------------------------------
	// Helper function
	std::string LoadTextFile(const std::string &filename)
	{
		std::ifstream infile(filename.c_str());
		std::ostringstream contents;
		if (infile.good())
		{
			contents << infile.rdbuf();
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
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestScribeVS.txt")));
		// Scribe fragment shaders
		if (res) res = scribe->AddFragmentShader(
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestScribeFS.txt")));
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
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestRaycasterVS.txt")));
		// raycaster fragment shaders
		if (res) res = raycaster->AddFragmentShader(
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestRaycasterFS.txt")));
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
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestPainterVS.txt")));
		// painter fragment shaders
		if (res) res = painter->AddFragmentShader(
			AddShaderDefines(LoadTextFile(
			"D:\\Temp\\VFE\\TestPainterFS.txt")));
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
