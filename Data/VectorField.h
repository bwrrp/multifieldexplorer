#pragma once

#include <QString>

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

	protected:
		VectorField();

		NQVTK::Scene *scene;
	};
}
