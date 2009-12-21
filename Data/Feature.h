#pragma once

#include <string>
#include <vector>

#include <NQVTK/Math/Vector3.h>

class GLProgram;

namespace MFE
{
	class Feature
	{
	public:
		bool enabled;
		NQVTK::Vector3 examplePos;
		std::vector<float> weights;
		float startThreshold;
		float endThreshold;
		float power;
		float stretch;

		// TODO: add more visualization style parameters
		NQVTK::Vector3 color;

		Feature();

		void SetupProgram(GLProgram *program, int index) const;

		// TODO: add a way to generate UI for controlling the feature
	};
}
