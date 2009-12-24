#include "Field.h"
#include "Field.moc"

#include "DataTransform.h"

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

#include <QDir>
#include <QFileInfo>

#include <cassert>
#include <cmath>
#include <fstream>
#include <string>

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
			if (volume) volume->Delete();
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
	Field::Field(DataTransform *transform) : transform(transform)
	{
		assert(transform != 0);
		qDebug("Loading field: dim=%d (original %d)", 
			transform->GetReducedDimension(), 
			transform->GetOriginalDimension());

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

		// Set up scene
		scene = new NQVTK::Scene();

		numComponents = 0;

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
		// TODO: add some way to specify property names in the field file

		std::ifstream fieldFile(filename.toUtf8());
		if (!fieldFile.is_open())
		{
			qDebug("Error opening field file!");
			return 0;
		}

		// Filenames in the field file are relative to the file's location
		QFileInfo fi(filename);
		QDir::setCurrent(fi.absolutePath());

		// The first line is the transform
		std::string transformFileName;
		getline(fieldFile, transformFileName);

		// Load the transform
		DataTransform *transform = DataTransform::Load(transformFileName);

		if (!transform)
		{
			qDebug("Error loading transform!");
			return 0;
		}

		// Create the field
		Field *field = new Field(transform);

		// Continue parsing the field file to find the component files
		while (fieldFile.good())
		{
			std::string componentFileName;
			getline(fieldFile, componentFileName);
			if (componentFileName == "") continue;
			vtkImageData *vtkVolume = LoadVolume(componentFileName.c_str());
			if (vtkVolume)
			{
				field->AddComponentVolume(vtkVolume);
				vtkVolume->Delete();
			}
			else
			{
				qDebug("Error loading component file!");
			}
		}

		// Field complete?
		// TODO: check number of components
		if (field->IsOk())
		{
			return field;
		}
		else
		{
			qDebug("Number of field components does not match transform!");
			delete field;
			return 0;
		}
	}

	// ------------------------------------------------------------------------
	void Field::AddComponentVolume(vtkImageData *vtkVolume)
	{
		assert(vtkVolume != 0);

		// Create bounding geometry
		vtkPolyData *vtkBB = CreateBoundingBox(vtkVolume);

		// Add to scene
		// TODO: all volumes should share the same bounding box
		NQVTK::Renderable *bb = new NQVTK::PolyData(vtkBB);
		NQVTK::Volume *volume = NQVTK::ImageDataVolume::New(vtkVolume);
		bb->SetParamSet("volume", new NQVTK::VolumeParamSet(volume));
		bb->opacity = 0.1;
		scene->AddRenderable(bb);

		// Delete VTK data
		vtkBB->Delete();

		numComponents += vtkVolume->GetNumberOfScalarComponents();
	}

	// ------------------------------------------------------------------------
	bool Field::IsOk()
	{
		return numComponents == transform->GetReducedDimension();
	}

	// ------------------------------------------------------------------------
	GLTexture *Field::GetTransformTexture()
	{
		return transform->GetTexture();
	}

	// ------------------------------------------------------------------------
	int Field::GetOriginalDimension()
	{
		return transform->GetOriginalDimension();
	}

	// ------------------------------------------------------------------------
	int Field::GetReducedDimension()
	{
		return transform->GetReducedDimension();
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
