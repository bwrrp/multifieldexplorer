#include "SliceRenderer.h"

#include "Data/Field.h"

#include <GLBlaat/GLProgram.h>

#include <NQVTK/ParamSets/VolumeParamSet.h>
#include <NQVTK/Renderables/Renderable.h>
#include <NQVTK/Rendering/View.h>
#include <NQVTK/Rendering/Volume.h>

#include <QApplication>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
	SliceRenderer::SliceRenderer() : field(0)
	{
	}

	// ------------------------------------------------------------------------
	void SliceRenderer::Draw()
	{
		// Prepare the shader uniforms
		// TODO: extend the RenderStyle mechanism to include normal Renderers
		if (field)
		{
			shader->Start();
			field->SetupProgram(shader);
			shader->Stop();
		}

		Superclass::Draw();
	}

	// ------------------------------------------------------------------------
	void SliceRenderer::SceneChanged()
	{
		// Make sure each renderable has a volume paramset
		// TODO: simplify NQVTK mixed object / volume rendering?
		for (unsigned int i = 0; i < view->GetNumberOfRenderables(); ++i)
		{
			NQVTK::Renderable *renderable = view->GetRenderable(i);
			if (renderable)
			{
				NQVTK::VolumeParamSet *vps = 
					dynamic_cast<NQVTK::VolumeParamSet*>(
						renderable->GetParamSet("volume"));
				if (!vps)
				{
					// Add an empty volume ParamSet
					renderable->SetParamSet("volume", 
						new NQVTK::VolumeParamSet(0));
				}
			}
		}

		Superclass::SceneChanged();
	}

	// ------------------------------------------------------------------------
	void SliceRenderer::SetField(Field *field)
	{
		this->field = field;
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

	// ------------------------------------------------------------------------
	GLProgram *SliceRenderer::CreateSliceViewerShader()
	{
		GLProgram *shader = GLProgram::New();
		if (!shader) return false;
		bool ok = true;
		if (ok) ok = shader->AddVertexShader(
			AddShaderDefines(LoadShader("SliceViewerVS.txt")));
		// TODO: we need the actual object transform here
		if (ok) ok = shader->AddFragmentShader(
			AddShaderDefines(LoadShader("SliceViewerFS.txt")));
		if (ok) ok = shader->Link();
		if (!ok) 
		{
			delete shader;
			return 0;
		}
		return shader;
	}

	// ------------------------------------------------------------------------
	std::string SliceRenderer::AddShaderDefines(const std::string &shader)
	{
		// TODO: replace this with some general system to manage shader options
		std::ostringstream source;
		// TODO: adjust this based on the actual number of features
		source << "#define MFE_FEATURECOUNT 6\n";
		int propCount = 4;
		int origPropCount = 6;
		if (field)
		{
			propCount = field->GetReducedDimension();
			origPropCount = field->GetOriginalDimension();
		}
		source << "#define MFE_PROPERTYCOUNT " << propCount << "\n";
		source << "#define MFE_ORIGINALPROPERTYCOUNT " << origPropCount 
			<< "\n";
		source << shader;
		return source.str();
	}
}
