#include "Feature.h"

#include <GLBlaat/GLProgram.h>

#include <sstream>

namespace MFE
{
	// ------------------------------------------------------------------------
	Feature::Feature()
	{
		enabled = true;
		startThreshold = 0.0;
		endThreshold = 0.5;
		power = 2.0;
		stretch = 1.0;

		biasRelativeToMean = true;

		// Default color is white
		color = NQVTK::Vector3(1.0);
	}

	// ------------------------------------------------------------------------
	void Feature::SetupProgram(GLProgram *program, int index) const
	{
		// Get the name of this feature
		std::ostringstream feature;
		if (index < 0)
		{
			feature << "userFeature.";
		}
		else
		{
			feature << "userFeatures[" << index << "].";
		}
		program->SetUniform1i(feature.str() + "enabled", enabled ? 1 : 0);
		program->SetUniform3f(feature.str() + "examplePos", 
			static_cast<float>(examplePos.x), 
			static_cast<float>(examplePos.y), 
			static_cast<float>(examplePos.z));
		program->SetUniform1f(feature.str() + "startThreshold", 
			startThreshold);
		program->SetUniform1f(feature.str() + "endThreshold", endThreshold);
		program->SetUniform1f(feature.str() + "power", power);
		program->SetUniform1f(feature.str() + "stretch", stretch);
		program->SetUniform3f(feature.str() + "backgroundPos", 
			static_cast<float>(backgroundPos.x), 
			static_cast<float>(backgroundPos.y), 
			static_cast<float>(backgroundPos.z));
		program->SetUniform1i(feature.str() + "biasRelativeToMean", 
			biasRelativeToMean ? 1 : 0);
		// Setup mask
		int numDims = 0;
		for (unsigned int i = 0; i < mask.size(); ++i)
		{
			std::ostringstream mname;
			mname << feature.str() << "mask.v[" << i << "]";
			program->SetUniform1i(mname.str(), mask[i] ? 1 : 0);
			if (mask[i]) ++numDims;
		}
		// Number of dimensions left by the mask
		program->SetUniform1i(feature.str() + "numDims", numDims);

		program->SetUniform3f(feature.str() + "color", 
			static_cast<float>(color.x), 
			static_cast<float>(color.y), 
			static_cast<float>(color.z));
	}
}
