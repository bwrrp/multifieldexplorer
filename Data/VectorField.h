#pragma once

#include <QString>

#include <vector>

#include "Feature.h"

class GLProgram;

namespace NQVTK
{
	class Scene;
}

namespace VFE
{
	class VectorField
	{
	public:
		static VectorField *Load(const QString &filename);

		virtual ~VectorField();

		NQVTK::Scene *GetScene() { return scene; }
		const NQVTK::Scene *GetScene() const { return scene; }

		int GetNumberOfFeatures() const;
		void SetupFeatures(GLProgram *program) const;

		void SetCursorPos(const NQVTK::Vector3 &pos);
		void SetSelectedPos(const NQVTK::Vector3 &pos);

	protected:
		VectorField(NQVTK::Scene *scene);

		NQVTK::Scene *scene;
		std::vector<Feature> features;
	};
}
