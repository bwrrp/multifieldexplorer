#pragma once

#include <QString>

#include <vector>

#include <QObject>

#include "Feature.h"

class GLProgram;
class GLTexture;

class vtkImageData;

namespace NQVTK
{
	class Scene;
}

namespace MFE
{
	class DataTransform;

	class Field : public QObject
	{
		Q_OBJECT;

	public:
		static Field *Load(const QString &filename);

		virtual ~Field();

		void AddComponentVolume(vtkImageData *vtkVolume);
		bool IsOk();

		int GetOriginalDimension() const;
		int GetReducedDimension() const;

		DataTransform *GetTransform() { return transform; }

		NQVTK::Scene *GetScene() { return scene; }
		const NQVTK::Scene *GetScene() const { return scene; }

		int GetNumberOfFeatures() const;
		Feature *GetFeature(int i);
		void AddFeature();
		void RemoveFeature(int i);

		void SetupProgram(GLProgram *program) const;

		void EmitFeatureUpdated(int num);

	signals:
		void FeatureUpdated(int num);

	protected:
		Field(DataTransform *transform);

		DataTransform *transform;
		NQVTK::Scene *scene;
		int numComponents;

		std::vector<Feature> features;
		std::vector<NQVTK::Vector3> featureColors;
	};
}
