#include "Field.h"

#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkMetaImageWriter.h>
#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>

#include <itpp/itsignal.h>
#include <itpp/itstat.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

namespace PropertySpace
{
	// ------------------------------------------------------------------------
	Field::Field()
	{
	}

	// ------------------------------------------------------------------------
	Field::~Field()
	{
	}

	// ------------------------------------------------------------------------
	Field *Field::Load(const std::string &filename)
	{
		// Load the field
		vtkImageData *volume = LoadVolume(filename);
		if (!volume) return 0;

		Field *field = new Field();
		field->Init(volume);
		volume->Delete();

		return field;
	}

	// ------------------------------------------------------------------------
	void Field::Init(vtkImageData *volume)
	{
		assert(volume != 0);

		// Get size
		int dims[3];
		volume->GetDimensions(dims);
		width = dims[0];
		height = dims[1];
		depth = dims[2];

		// Get spacing and origin
		volume->GetSpacing(spacing);
		volume->GetOrigin(origin);

		// Build data matrix
		BuildPropertyMatrix(volume);
		CenterData();
	}

	// ------------------------------------------------------------------------
	void Field::DoPCA()
	{
		// Compute covariance matrix
		std::cout << "Computing covariance matrix..." << std::endl;
		itpp::mat covariance = itpp::cov(data);

		// Perform eigen analysis
		std::cout << "Computing eigenvectors..." << std::endl;
		itpp::vec unsortedEigVals;
		itpp::mat unsortedEigVecs;
		itpp::eig_sym(covariance, unsortedEigVals, unsortedEigVecs);

		// Sort by eigenvalue
		std::cout << "Sorting components..." << std::endl;
		itpp::ivec index = itpp::reverse(itpp::sort_index(unsortedEigVals));

		// Allocate space
		eigVals.set_size(unsortedEigVals.size());
		eigVecs.set_size(unsortedEigVecs.rows(), unsortedEigVecs.cols());

		// Sort and remove insignificant components
		int comp = 0;
		for (int col = 0; col < unsortedEigVecs.cols(); ++col)
		{
			itpp::vec evec = unsortedEigVecs.get_col(index(col));
			double eval = unsortedEigVals(index(col));
			const double epsilon = 0.0;
			if (eval > epsilon)
			{
				eigVecs.set_col(comp, evec);
				eigVals(comp) = eval;
				++comp;
			}
			else
			{
				if (eval < -epsilon)
				{
					std::cerr << "Warning! Negative eigenvalue: " << eval << 
						std::endl;
				}
				// Treat this eigenvalue as if it's 0 and discard the result
			}
		}
		// Discard zero-valued components
		eigVecs.set_size(eigVecs.rows(), comp, true);
		eigVals.set_size(comp, true);

		std::cout << "PCA complete! Found " << comp << 
			" significant components" << std::endl;

		// Report results
		for (int i = 0; i < comp; ++i)
		{
			std::cout << i + 1 << ": " << eigVecs.get_col(i) << 
				" (eval " << eigVals(i) << ")" << std::endl;
		}
	}

	// ------------------------------------------------------------------------
	void Field::Transform()
	{
		// The transformation matrix is based on scaled eigenvectors
		std::cout << "Buiding transformation..." << std::endl;
		basis = eigVecs;
		for (int i = 0; i < basis.cols(); ++i)
		{
			itpp::vec v = basis.get_col(i);
			v *= 1.0 / sqrt(eigVals(i));
			basis.set_col(i, v);
		}

		std::cout << "Transforming data..." << std::endl;
		data = data * basis;
	}

	// ------------------------------------------------------------------------
	void Field::Save(const std::string &filename, int comps)
	{
		if (comps <= 0) comps = data.cols();
		if (comps > data.cols()) comps = data.cols();
		std::cout << "Saving " << comps << " components..." << std::endl;

		// Save transform
		SaveDataTransform(filename, comps);

		// Prepare volume
		std::cout << "Preparing to save fields..." << std::endl;
		vtkSmartPointer<vtkImageData> volume = 
			vtkSmartPointer<vtkImageData>::New();
		volume->SetScalarTypeToFloat();
		volume->SetDimensions(width, height, depth);
		volume->SetNumberOfScalarComponents(4);
		volume->SetSpacing(spacing);
		volume->SetOrigin(origin);
		volume->AllocateScalars();
		float *field = static_cast<float*>(volume->GetScalarPointer());

		// Save the field in up-to-4-component files
		int startComp = 0;
		while (comps > 0)
		{
			// Copy the data for this volume
			if (comps < 4)
			{
				volume->SetNumberOfScalarComponents(comps);
				volume->AllocateScalars();
			}
			int numComps = volume->GetNumberOfScalarComponents();

			std::cout << "Saving field " << startComp / 4 << 
				"..." << std::endl;

			for (int i = 0; i < width * height * depth; ++i)
			{
				for (int c = 0; c < numComps; ++c)
				{
					field[i * numComps + c] = data(i, startComp + c);
				}
			}

			// Write the volume to disk
			std::ostringstream file;
			file << filename << "_comp" << startComp / 4 << ".mha";

			vtkSmartPointer<vtkMetaImageWriter> writer = 
				vtkSmartPointer<vtkMetaImageWriter>::New();
			writer->SetFileName(file.str().c_str());
			writer->SetInput(volume);
			writer->Write();

			// Next file / set of components
			startComp += 4;
			comps -= 4;
		}
	}

	// ------------------------------------------------------------------------
	vtkImageData *Field::LoadVolume(const std::string &filename)
	{
		// Use a vtk reader to load the vector field
		vtkImageData *volume = 0;
		std::string ext = filename.substr(filename.rfind('.'));
		if (ext == ".mha" || ext == ".mhd")
		{
			vtkSmartPointer<vtkMetaImageReader> reader = 
				vtkSmartPointer<vtkMetaImageReader>::New();
			reader->SetFileName(filename.c_str());
			reader->Update();
			volume = reader->GetOutput();
			volume->Register(0);
		}
		else if (ext == ".vti")
		{
			vtkSmartPointer<vtkXMLImageDataReader> reader = 
				vtkSmartPointer<vtkXMLImageDataReader>::New();
			reader->SetFileName(filename.c_str());
			reader->Update();
			volume = reader->GetOutput();
			volume->Register(0);
		}
		else
		{
			std::cerr << "Unsupported file type: '" << ext << "'" << std::endl;
			return 0;
		}

		return volume;
	}

	// ------------------------------------------------------------------------
	void Field::BuildPropertyMatrix(vtkImageData *volume)
	{
		assert(volume->GetScalarType() == VTK_FLOAT);
		float *field = static_cast<float*>(volume->GetScalarPointer());
		int comps = volume->GetNumberOfScalarComponents();

		std::cout << "Building matrix..." << std::endl;
		data.set_size(width * height * depth, comps);
		for (int row = 0; row < width * height * depth; ++row)
		{
			for (int comp = 0; comp < comps; ++comp)
			{
				data(row, comp) = *field;
				++field;
			}
		}
	}

	// ------------------------------------------------------------------------
	void Field::CenterData()
	{
		// Compute mean
		mean.set_size(data.cols());
		for (int i = 0; i < data.cols(); ++i)
		{
			mean(i) = itpp::mean(data.get_col(i));
		}
		// Center data
		for (int i = 0; i < data.rows(); ++i)
		{
			data.set_row(i, data.get_row(i) - mean);
		}
	}

	// ------------------------------------------------------------------------
	void Field::SaveDataTransform(const std::string &filename, int comps)
	{
		// Save transformation matrix
		std::cout << "Saving transform..." << std::endl;
		itpp::mat transform = basis.get(0, basis.rows() - 1, 0, comps - 1);

		std::ostringstream fullFileName;
		fullFileName << filename << "_transform.dat";

		std::ofstream transformFile(fullFileName.str().c_str(), 
			std::ios::out | std::ios::binary | std::ios::trunc);

		if (transformFile.is_open())
		{
			// Save number of basis vectors
			int val = transform.cols();
			transformFile.write((char*)&val, sizeof(val));
			// Save dimensionality of the original space
			val = transform.rows();
			transformFile.write((char*)&val, sizeof(val));
			// Save data - each column is a basis vector
			for (int col = 0; col < transform.cols(); ++col)
			{
				for (int row = 0; row < transform.rows(); ++row)
				{
					double v = transform(row, col);
					transformFile.write((char*)&v, sizeof(v));
				}
			}
			// Save mean (in original space)
			for (int i = 0; i < mean.size(); ++i)
			{
				double v = mean(i);
				transformFile.write((char*)&v, sizeof(v));
			}
		}
	}
}
