#include "DataTransform.h"

#include <GLBlaat/GLTextureRectangle.h>

#include <fstream>

namespace MFE
{
	// ------------------------------------------------------------------------
	DataTransform *DataTransform::Load(const std::string &filename)
	{
		// Load the file
		std::ifstream transformFile(filename.c_str(), 
			std::ios::in | std::ios::binary);
		if (!transformFile.is_open()) return 0;

		// Read dimensions
		int dimOriginal;
		int dimReduced;
		transformFile.read((char*)&dimReduced, sizeof(dimReduced));
		transformFile.read((char*)&dimOriginal, sizeof(dimOriginal));

		// Read data
		std::vector<float> data;
		// There should be dimReduced basis vectors + mean, min and max vectors
		int numFloats = dimOriginal * (dimReduced + 3);
		data.resize(numFloats);
		transformFile.read((char*)&data[0], numFloats * sizeof(float));

		// Did anything go wrong?
		if (transformFile.fail())
		{
			return 0;
		}

		return new DataTransform(dimOriginal, dimReduced, data);
	}

	// ------------------------------------------------------------------------
	DataTransform::DataTransform(int dimOriginal, int dimReduced, 
		const std::vector<float> &data) 
		: dimOriginal(dimOriginal), dimReduced(dimReduced), data(data)
	{
	}

	// ------------------------------------------------------------------------
	DataTransform::~DataTransform()
	{
	}

	// ------------------------------------------------------------------------
	GLTexture *DataTransform::GetTexture()
	{
		// Create a texture from the data
		return GLTextureRectangle::New(
			dimOriginal, dimReduced + 3, GL_LUMINANCE32F_ARB, 
			GL_LUMINANCE, GL_FLOAT, (void*)&data[0]);
	}
}
