#include "Field.h"
#include "Field.moc"

#include <NQVTK/ParamSets/VolumeParamSet.h>

#include <NQVTK/Rendering/Scene.h>
#include <NQVTK/Rendering/ImageDataVolume.h>

#include <NQVTK/Renderables/PolyData.h>

#include <GLBlaat/GLProgram.h>

#include <vtkCubeSource.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTriangleFilter.h>
#include <vtkXMLImageDataReader.h>

#include <QFileInfo>

#include <cmath>

namespace MFE
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
			(volume->GetDataDimension() == 3);
		if (!ok) 
		{
			qDebug("Not a volume!");
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
	Field::Field(NQVTK::Scene *scene) : scene(scene)
	{
		// Create a set of feature colors
		// TODO: integrate this into the GUI at some point
		featureColors.push_back(NQVTK::Vector3(1.0, 1.0, 1.0));
		featureColors.push_back(NQVTK::Vector3(1.0, 0.5, 0.0));
		featureColors.push_back(NQVTK::Vector3(0.0, 0.5, 1.0));
		featureColors.push_back(NQVTK::Vector3(1.0, 0.0, 0.5));
		featureColors.push_back(NQVTK::Vector3(0.0, 1.0, 0.5));
		featureColors.push_back(NQVTK::Vector3(0.5, 1.0, 0.0));
		featureColors.push_back(NQVTK::Vector3(0.5, 0.0, 1.0));
		featureColors.push_back(NQVTK::Vector3(1.0, 1.0, 1.0));

		// Add Cursor feature
		AddFeature();
	}

	// ------------------------------------------------------------------------
	Field::~Field()
	{
		delete scene;
	}

	// ------------------------------------------------------------------------
	Field *Field::Load(const QString &filename)
	{
		// TODO: load field description file, containing...
		//        - references to field component files
		//        - a reference to the transform basis / mean file
		//        - a (reference to a?) list of property names (for GUI)

		// TODO: load each component file
		// TODO: determine number of components, set up shaders
		// TODO: load transform, check number of components
		// TODO: setup property widgets

		// Load the field
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

		// Create the Field instance
		Field *vf = new Field(scene);

		return vf;
	}

	// ------------------------------------------------------------------------
	int Field::GetNumberOfFeatures() const
	{
		return features.size();
	}

	// ------------------------------------------------------------------------
	Feature *Field::GetFeature(int i)
	{
		if (i < 0 || i >= GetNumberOfFeatures()) return 0;
		return &features[i];
	}

	// ------------------------------------------------------------------------
	void Field::AddFeature()
	{
		features.push_back(Feature());
		// Assign a color from the palette
		unsigned int i = features.size() - 1;
		features[i].color = 
			featureColors[std::min(i, featureColors.size() - 1)];
	}

	// ------------------------------------------------------------------------
	void Field::RemoveFeature(int i)
	{
		if (i < 0 || i >= GetNumberOfFeatures()) return;
		features.erase(features.begin() + i);
	}

	// ------------------------------------------------------------------------
	void Field::SetupFeatures(GLProgram *program) const
	{
		int numFeatures = GetNumberOfFeatures();
		program->SetUniform1i("numActiveFeatures", numFeatures);
		for (int i = 0; i < numFeatures; ++i)
		{
			features[i].SetupProgram(program, i);
		}
	}

	// ------------------------------------------------------------------------
	void Field::EmitFeatureUpdated(int num)
	{
		emit FeatureUpdated(num);
	}
}
