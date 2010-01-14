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
		float startThreshold;
		float endThreshold;
		float power;
		float stretch;
		std::vector<bool> mask;
		NQVTK::Vector3 backgroundPos;
		bool biasRelativeToMean;

		// TODO: add more visualization style parameters
		NQVTK::Vector3 color;

		Feature();

		void SetupProgram(GLProgram *program, int index) const;

		// TODO: add a way to generate UI for controlling the feature
	};
}
