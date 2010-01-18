#include "FeedbackRenderer.h"

#include "Data/Field.h"
#include "Data/Feature.h"

#include <NQVTK/Renderables/Renderable.h>
#include <NQVTK/Rendering/View.h>

#include <GLBlaat/GLFramebuffer.h>
#include <GLBlaat/GLProgram.h>
#include <GLBlaat/GLTextureManager.h>

#include <QApplication>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	FeedbackRenderer::FeedbackRenderer() : currentFeature(0), shader(0)
	{
	}

	// ------------------------------------------------------------------------
	FeedbackRenderer::~FeedbackRenderer()
	{
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::Draw()
	{
		Clear();

		if (currentFeature)
		{
			// Projection pass

			// TODO: Set up shader, transform texture
			// TODO: Pass feature example pos & bias vector type
			// TODO: Render to FBO

			// Visualization pass (color mapping)
			if (fboTarget) fboTarget->Bind();

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_LIGHTING);
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);

			shader->Start();
			shader->SetUniform1i("viewportWidth", viewportWidth);
			shader->SetUniform1i("dimOriginal", dimOriginal);
			shader->SetUniform1i("dimReduced", dimReduced);

			// Assume the field is in the first renderable
			// TODO: add support for more than 4 components
			NQVTK::Renderable *ren = view->GetRenderable(0);
			if (ren)
			{
				ren->ApplyParamSets(shader, tm);
			}

			currentFeature->SetupProgram(shader, -1);

			tm->SetupProgram(shader);
			tm->Bind();

			// Full-screen quad
			glBegin(GL_QUADS);
			glVertex3d(-1.0, -1.0, 0.0);
			glVertex3d(1.0, -1.0, 0.0);
			glVertex3d(1.0, 1.0, 0.0);
			glVertex3d(-1.0, 1.0, 0.0);
			glEnd();

			tm->Unbind();

			shader->Stop();

			glPopAttrib();

			if (fboTarget) fboTarget->Unbind();
		}
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::SetField(Field *field)
	{
		if (field)
		{
			dimOriginal = field->GetOriginalDimension();
			dimReduced = field->GetReducedDimension();

			SetScene(field->GetScene());
		}
		else
		{
			SetScene(0);
		}

		currentFeature = 0;
	}

	// ------------------------------------------------------------------------
	void FeedbackRenderer::SetFeature(Feature *feature)
	{
		currentFeature = feature;
	}

	// ------------------------------------------------------------------------
	bool FeedbackRenderer::Initialize()
	{
		if (!Superclass::Initialize()) return false;

		// TODO: create and initialize FBO for the projected example / vector
		
		// Create shader
		delete shader;
		shader = GLProgram::New();
		if (!shader) return false;
		bool ok = shader->AddVertexShader(
			LoadShader("FeedbackVS.txt"));
		// TODO: we need the actual object transform here
		if (ok) ok = shader->AddFragmentShader(
			LoadShader("FeedbackFS.txt"));
		if (ok) ok = shader->Link();
		if (!ok) 
		{
			delete shader;
			shader = 0;
			return false;
		}

		return true;
	}
}
