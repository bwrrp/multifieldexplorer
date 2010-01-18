#pragma once

#include <string>
#include <vector>

class GLProgram;

namespace MFE
{
	class DataTransform
	{
	public:
		static DataTransform *Load(const std::string &filename);

		virtual ~DataTransform();

		int GetOriginalDimension() { return dimOriginal; }
		int GetReducedDimension() { return dimReduced; }

		void SetupProgram(GLProgram *program);

	protected:
		DataTransform(int dimOriginal, int dimReduced, 
			const std::vector<float> &data);

		const int dimOriginal;
		const int dimReduced;

		const std::vector<float> data;
	};
}
