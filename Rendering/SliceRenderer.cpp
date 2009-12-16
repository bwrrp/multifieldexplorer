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
	SliceRenderer::SliceRenderer() : field(0)
	{
		unitSize = 1.0;
		kernelSize = 1.0;
	}

	// ------------------------------------------------------------------------
	void SliceRenderer::Draw()
	{
		// Prepare the shader uniforms
		// TODO: extend the RenderStyle mechanism to include normal Renderers
		shader->Start();
		if (field) field->SetupFeatures(shader);
		shader->SetUniform1f("kernelSize", kernelSize * unitSize);
		shader->Stop();

		Superclass::Draw();
	}

	// ------------------------------------------------------------------------
	void SliceRenderer::SceneChanged()
	{
		// Compute unitSize
		unitSize = 1000000.0;
		for (unsigned int i = 0; i < view->GetNumberOfRenderables(); ++i)
		{
			NQVTK::Renderable *renderable = view->GetRenderable(i);
			if (renderable)
			{
				NQVTK::VolumeParamSet *vps = 
					dynamic_cast<NQVTK::VolumeParamSet*>(
						renderable->GetParamSet("volume"));
				if (vps)
				{
					NQVTK::Volume *volume = vps->GetVolume();
					if (volume)
					{
						// Compute spacings
						NQVTK::Vector3 size = 
							volume->GetOriginalSize();
						double spX = size.x / static_cast<double>(
							volume->GetWidth() - 1);
						double spY = size.y / static_cast<double>(
							volume->GetHeight() - 1);
						double spZ = size.z / static_cast<double>(
							volume->GetDepth() - 1);
						// Update unitSize if any are smaller
						if (spX < unitSize) unitSize = spX;
						if (spY < unitSize) unitSize = spY;
						if (spZ < unitSize) unitSize = spZ;
					}
				}
				else
				{
					// Add an empty volume ParamSet
					renderable->SetParamSet("volume", 
						new NQVTK::VolumeParamSet(0));
				}
			}
		}
		assert(unitSize > 0.0);

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
}
