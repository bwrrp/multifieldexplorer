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

		// Default color is white
		color = NQVTK::Vector3(1.0);
	}

	// ------------------------------------------------------------------------
	void Feature::SetupProgram(GLProgram *program, int index) const
	{
		// Get the name of this feature
		std::ostringstream feature;
		feature << "userFeatures[" << index << "].";
		program->SetUniform1i(feature.str() + "enabled", enabled ? 1 : 0);
		program->SetUniform3f(feature.str() + "examplePos", 
			static_cast<float>(examplePos.x), 
			static_cast<float>(examplePos.y), 
			static_cast<float>(examplePos.z));
		// Setup weights
		for (unsigned int i = 0; i < weights.size(); ++i)
		{
			std::ostringstream wname;
			wname << feature.str() << "weights.v[" << i << "]";
			program->SetUniform1f(wname.str(), weights[i]);
		}
		// Setup other parameters
		program->SetUniform1f(feature.str() + "startThreshold", 
			startThreshold);
		program->SetUniform1f(feature.str() + "endThreshold", endThreshold);
		program->SetUniform1f(feature.str() + "power", power);
		program->SetUniform1f(feature.str() + "stretch", stretch);

		program->SetUniform3f(feature.str() + "color", 
			static_cast<float>(color.x), 
			static_cast<float>(color.y), 
			static_cast<float>(color.z));
	}
}
