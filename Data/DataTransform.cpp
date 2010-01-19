#include "DataTransform.h"

#include <GLBlaat/GLProgram.h>

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
	void DataTransform::SetupProgram(GLProgram *program)
	{
		// Setup transform data uniforms
		int n = dimReduced * dimOriginal;
		program->SetUniform1fv("dataTransform", n, &data[0]);
		program->SetUniform1fv("dataMean", dimOriginal, &data[n]);
		n += dimOriginal;
		program->SetUniform1fv("dataMin", dimOriginal, &data[n]);
		n += dimOriginal;
		program->SetUniform1fv("dataMax", dimOriginal, &data[n]);
	}
}
