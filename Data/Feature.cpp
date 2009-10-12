#include "Feature.h"

#include <GLBlaat/GLProgram.h>

#include <sstream>

namespace VFE
{
	// ------------------------------------------------------------------------
	Property::Property(const std::string &name, PropertyType type)
		: name(name), type(type) 
	{
	}

	// ------------------------------------------------------------------------
	int Property::GetSize() const
	{
		switch (type)
		{
		case Float:
			return 1;
			break;
		case Vec3:
			return 3;
			break;
		default:
			return 0;
		}
	}

	// ------------------------------------------------------------------------
	std::vector<Property> FeatureVector::properties = std::vector<Property>();

	// ------------------------------------------------------------------------
	FeatureVector::FeatureVector()
	{
		if (properties.size() == 0)
		{
			properties.push_back(Property("position", Property::Vec3));
			properties.push_back(Property("vector", Property::Vec3));
			properties.push_back(Property("direction", Property::Vec3));
			properties.push_back(Property("magnitude", Property::Float));
			properties.push_back(Property("growth", Property::Float));
			properties.push_back(Property("divergence", Property::Float));
			properties.push_back(Property("curl", Property::Vec3));
		}

		// Compute total size
		int totalSize = 0;
		for (std::vector<Property>::const_iterator it = properties.begin(); 
			it != properties.end(); ++it)
		{
			totalSize += it->GetSize();
		}
		values.resize(totalSize, 0.0f);
	}

	// ------------------------------------------------------------------------
	void FeatureVector::SetupProgram(GLProgram *program, 
		const std::string &feature) const
	{
		int v = 0;
		for (std::vector<Property>::const_iterator it = properties.begin(); 
			it != properties.end(); ++it)
		{
			switch (it->type)
			{
			case Property::Float:
				program->SetUniform1f(feature + it->name, values[v]);
				break;
			case Property::Vec3:
				program->SetUniform3f(feature + it->name, 
					values[v], values[v + 1], values[v + 2]);
				break;
			}
			v += it->GetSize();
		}
	}

	// ------------------------------------------------------------------------
	Feature::Feature()
	{
		enabled = true;
		startThreshold = 0.0;
		endThreshold = 0.5;
		power = 2.0;

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
		mask.SetupProgram(program, feature.str() + "mask.");
		program->SetUniform1f(feature.str() + "startThreshold", 
			startThreshold);
		program->SetUniform1f(feature.str() + "endThreshold", endThreshold);
		program->SetUniform1f(feature.str() + "power", power);

		program->SetUniform3f(feature.str() + "color", 
			static_cast<float>(color.x), 
			static_cast<float>(color.y), 
			static_cast<float>(color.z));
	}
}
