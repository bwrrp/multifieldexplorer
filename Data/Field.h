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

	protected:
		int width, height, depth;
		itpp::mat data;

		itpp::vec eigVals;
		itpp::mat eigVecs;

		Field(vtkImageData *volume);

		void BuildPropertyMatrix(vtkImageData *volume);
	};
}
