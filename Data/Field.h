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

		void Init(vtkImageData *volume);

		void DoPCA();

		void Transform();

		void Save(const std::string &filename, int comps);

	protected:
		int width, height, depth;
		double spacing[3];
		double origin[3];

		itpp::mat data;
		itpp::vec mean;
		itpp::vec min;
		itpp::vec max;

		itpp::vec eigVals;
		itpp::mat eigVecs;
		itpp::mat basis;

		Field();

		static vtkImageData *LoadVolume(const std::string &filename);

		virtual void BuildPropertyMatrix(vtkImageData *volume);

		void CenterData();

		void SaveDataTransform(const std::string &filename, int comps);
	};
}
