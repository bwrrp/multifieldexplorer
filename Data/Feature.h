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
		FeatureVector();

		void SetupProgram(GLProgram *program, const std::string &feature);

	protected:
		// Property descriptions are shared by all FeatureVectors
		static std::vector<Property> properties;
		std::vector<float> values;
	};

	class Feature
	{
	public:
		bool enabled;
		NQVTK::Vector3 examplePos;
		FeatureVector mask;
		float startThreshold;
		float endThreshold;
		// TODO: add visualization style parameters

		void SetupProgram(GLProgram *program, int index);
	};
}
