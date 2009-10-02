#pragma once

#include <QString>

#include <vector>

#include <QObject>

#include "Feature.h"

class GLProgram;

namespace NQVTK
{
	class Scene;
}

namespace VFE
{
	class VectorField : public QObject
	{
		Q_OBJECT;

	public:
		static VectorField *Load(const QString &filename);

		virtual ~VectorField();

		NQVTK::Scene *GetScene() { return scene; }

		int GetNumberOfFeatures() const;
		Feature *GetFeature(int i);
		void AddFeature();
		void RemoveFeature(int i);

		void SetupFeatures(GLProgram *program) const;

		void EmitFeatureUpdated(int num);

	signals:
		void FeatureUpdated(int num);

	protected:
		VectorField(NQVTK::Scene *scene);

		NQVTK::Scene *scene;
		std::vector<Feature> features;
	};
}
