#include "Field.h"

#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>

#include <itpp/itsignal.h>
#include <itpp/itstat.h>

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

		int numComps = volume->GetNumberOfScalarComponents();

		// Build data matrix
		std::cout << "Building matrix..." << std::endl;
		data.set_size(width * height * depth, numComps);
		int row = 0;
		for (int z = 0; z < depth; ++z)
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					for (int comp = 0; comp < numComps; ++comp)
					{
						data(row, comp) = 
							volume->GetScalarComponentAsDouble(x, y, z, comp);
					}
					++row;
				}
			}
		}
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
}
