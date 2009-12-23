#pragma once

#include <string>
#include <vector>

class GLTexture;

namespace MFE
{
	class DataTransform
	{
	public:
		static DataTransform *Load(const std::string &filename);

		virtual ~DataTransform();

		int GetOriginalDimension() { return dimOriginal; }
		int GetReducedDimension() { return dimReduced; }

		GLTexture *GetTexture();

	protected:
		DataTransform(int dimOriginal, int dimReduced, 
			const std::vector<float> &data);

		const int dimOriginal;
		const int dimReduced;

		const std::vector<float> data;
	};
}
