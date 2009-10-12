#pragma once

#include <string>
#include <vector>

#include <NQVTK/Math/Vector3.h>

class GLProgram;

namespace VFE
{
	class Property
	{
	public:
		std::string name;
		enum PropertyType
		{
			Float, 
			Vec3
		} type;

		Property(const std::string &name, PropertyType type);

		int GetSize() const;
	};

	class FeatureVector
	{
	public:
		// Property descriptions are shared by all FeatureVectors
		static std::vector<Property> properties;
		std::vector<float> values;

		FeatureVector();

		void SetupProgram(GLProgram *program, 
			const std::string &feature) const;
	};

	class Feature
	{
	public:
		bool enabled;
		NQVTK::Vector3 examplePos;
		FeatureVector mask;
		float startThreshold;
		float endThreshold;
		float power;

		// TODO: add more visualization style parameters
		NQVTK::Vector3 color;

		Feature();

		void SetupProgram(GLProgram *program, int index) const;

		// TODO: add a way to generate UI for controlling the feature
	};
}
