#include "VectorField.h"

#include <NQVTK/ParamSets/VolumeParamSet.h>

#include <NQVTK/Rendering/Scene.h>
#include <NQVTK/Rendering/ImageDataVolume.h>

#include <NQVTK/Renderables/PolyData.h>

#include <vtkCubeSource.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTriangleFilter.h>
#include <vtkXMLImageDataReader.h>

#include <QFileInfo>

namespace VFE
{
	// ------------------------------------------------------------------------
	// VTK Helpers
	vtkImageData *LoadVolume(const QString &filename)
	{
		vtkImageData *volume = 0;
		QFileInfo fi(filename);
		// TODO: handle errors gracefully
		if (fi.suffix() == "mha" || fi.suffix() == "mhd")
		{
			vtkSmartPointer<vtkMetaImageReader> reader = 
				vtkSmartPointer<vtkMetaImageReader>::New();
			reader->SetFileName(filename.toUtf8());
			reader->Update();
			volume = reader->GetOutput();
			volume->Register(0);
		}
		else if (fi.suffix() == "vti")
		{
			vtkSmartPointer<vtkXMLImageDataReader> reader = 
				vtkSmartPointer<vtkXMLImageDataReader>::New();
			reader->SetFileName(filename.toUtf8());
			reader->Update();
			volume = reader->GetOutput();
			volume->Register(0);
		}
		else
		{
			qDebug("Unsupported file type!");
		}

		// Quick sanity check
		bool ok = volume != 0 && 
			(volume->GetDataDimension() == 3) && 
			(volume->GetNumberOfScalarComponents() == 3);
		if (!ok) 
		{
			qDebug("Not a vector volume!");
			volume->Delete();
			volume = 0;
		}

		return volume;
	}

	// ------------------------------------------------------------------------
	vtkPolyData *CreateBoundingBox(vtkImageData *volume)
	{
		vtkSmartPointer<vtkCubeSource> cube = 
			vtkSmartPointer<vtkCubeSource>::New();
		cube->SetBounds(volume->GetBounds());

		// Triangulate, NQVTK doesn't like quads
		vtkSmartPointer<vtkTriangleFilter> tri = 
			vtkSmartPointer<vtkTriangleFilter>::New();
		tri->SetInputConnection(cube->GetOutputPort());
		tri->Update();

		vtkPolyData *bb = tri->GetOutput();
		bb->Register(0);

		return bb;
	}

	// ------------------------------------------------------------------------
	VectorField::VectorField(NQVTK::Scene *scene) : scene(scene)
	{
		// Create cursor feature
		features.push_back(Feature());
		// Create selection feature
		features.push_back(Feature());
	}

	// ------------------------------------------------------------------------
	VectorField::~VectorField()
	{
		delete scene;
	}

	// ------------------------------------------------------------------------
	VectorField *VectorField::Load(const QString &filename)
	{
		// Load the vector field
		vtkImageData *vtkvolume = LoadVolume(filename);
		if (!vtkvolume)
		{
			// Returns null when errors occur
			return 0;
		}

		// Create bounding geometry
		vtkPolyData *vtkbb = CreateBoundingBox(vtkvolume);

		// Assemble NQVTK scene
		NQVTK::Scene *scene = new NQVTK::Scene();
		NQVTK::Renderable *bb = new NQVTK::PolyData(vtkbb);
		NQVTK::Volume *volume = NQVTK::ImageDataVolume::New(vtkvolume);
		bb->SetParamSet("volume", new NQVTK::VolumeParamSet(volume));
		bb->opacity = 0.1;
		scene->AddRenderable(bb);

		// Delete VTK data
		vtkvolume->Delete();
		vtkbb->Delete();

		// Create the VectorField instance
		VectorField *vf = new VectorField(scene);

		return vf;
	}

	// ------------------------------------------------------------------------
	int VectorField::GetNumberOfFeatures() const
	{
		return features.size();
	}

	// ------------------------------------------------------------------------
	Feature *VectorField::GetFeature(int i)
	{
		if (i < 0 || i >= GetNumberOfFeatures()) return 0;
		return &features[i];
	}

	// ------------------------------------------------------------------------
	void VectorField::SetupFeatures(GLProgram *program) const
	{
		int numFeatures = GetNumberOfFeatures();
		for (int i = 0; i < numFeatures; ++i)
		{
			features[i].SetupProgram(program, i);
		}
	}

	// ------------------------------------------------------------------------
	void VectorField::SetCursorPos(const NQVTK::Vector3 &pos)
	{
		features[0].examplePos = pos;
	}

	// ------------------------------------------------------------------------
	void VectorField::SetSelectedPos(const NQVTK::Vector3 &pos)
	{
		// TODO: replace this when we have UI dealing with multiple features
		features[1].examplePos = pos;
	}
}
