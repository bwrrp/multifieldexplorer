#pragma once

#include <itpp/itbase.h>

#include <string>

class vtkImageData;

namespace PropertySpace
{
	class Field
	{
	public:
		static Field *Load(const std::string &filename);
		virtual ~Field();

		void DoPCA();

		void Transform();

		void Save(const std::string &filename, int comps);

	protected:
		int width, height, depth;
		double spacing[3];
		double origin[3];

		itpp::mat data;
		itpp::vec mean;

		itpp::vec eigVals;
		itpp::mat eigVecs;

		Field(vtkImageData *volume);

		void BuildPropertyMatrix(vtkImageData *volume);
	};
}
