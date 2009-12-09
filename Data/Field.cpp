#include "Field.h"

#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>

#include <itpp/itsignal.h>
#include <itpp/itstat.h>

#include "Vector3.h"

#include <cassert>
#include <iostream>

namespace PropertySpace
{
	// ------------------------------------------------------------------------
	Field::Field(vtkImageData *volume)
	{
		assert(volume != 0);

		// Get size
		int dims[3];
		volume->GetDimensions(dims);
		width = dims[0];
		height = dims[1];
		depth = dims[2];

		// We assume the volume is a vector volume
		int numComps = volume->GetNumberOfScalarComponents();
		assert(numComps == 3);

		// Build data matrix
		BuildPropertyMatrix(volume);
	}

	// ------------------------------------------------------------------------
	Field::~Field()
	{
	}

	// ------------------------------------------------------------------------
	Field *Field::Load(const std::string &filename)
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

		Field *field = new Field(volume);
		volume->Delete();
		return field;
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
			const double epsilon = 0.000001;
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
		itpp::mat basis = eigVecs;
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
	void Field::BuildPropertyMatrix(vtkImageData *volume)
	{
		// vector, direction and magnitude
		// TODO: implement Gaussian convolution for derivatives
		const int numComps = 3 + 3 + 1;

		std::cout << "Building matrix..." << std::endl;
		data.set_size(width * height * depth, numComps);
		int row = 0;
		for (int z = 0; z < depth; ++z)
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					NQVTK::Vector3 v(
						volume->GetScalarComponentAsDouble(x, y, z, 0), 
						volume->GetScalarComponentAsDouble(x, y, z, 1), 
						volume->GetScalarComponentAsDouble(x, y, z, 2));

					// Vector
					data(row, 0) = v.x;
					data(row, 1) = v.y;
					data(row, 2) = v.z;

					// Direction
					NQVTK::Vector3 d = v.normalized();
					data(row, 3) = d.x;
					data(row, 4) = d.y;
					data(row, 5) = d.z;

					// Magnitude
					data(row, 6) = v.length();

					++row;
				}
			}
		}

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
}
