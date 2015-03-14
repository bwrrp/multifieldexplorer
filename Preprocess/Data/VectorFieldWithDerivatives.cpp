#include "VectorFieldWithDerivatives.h"

#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageGradient.h>
#include <vtkSmartPointer.h>

#include "Vector3.h"
#include "Matrix3x3.h"

#include <cassert>

namespace PropertySpace
{
	// ------------------------------------------------------------------------
	VectorFieldWithDerivatives *VectorFieldWithDerivatives::Load(
		const std::string &filename)
	{
		// Load the field
		vtkImageData *volume = LoadVolume(filename);
		if (!volume) return 0;

		VectorFieldWithDerivatives *field = new VectorFieldWithDerivatives();
		field->Init(volume);
		volume->Delete();

		return field;
	}

	// ------------------------------------------------------------------------
	VectorFieldWithDerivatives::VectorFieldWithDerivatives()
	{
	}

	// ------------------------------------------------------------------------
	void VectorFieldWithDerivatives::BuildPropertyMatrix(vtkImageData *volume)
	{
		// We assume the volume is a vector volume
		int numComps = volume->GetNumberOfScalarComponents();
		assert(numComps == 3);

		// TODO: implement Gaussian convolution for derivatives

		// Compute derivatives
		vtkImageData *gradients[3];
		double *gfield[3];
		for (int i = 0; i < 3; ++i)
		{
			vtkSmartPointer<vtkImageExtractComponents> comps = 
				vtkSmartPointer<vtkImageExtractComponents>::New();
			comps->SetInput(volume);
			comps->SetComponents(i);
			vtkSmartPointer<vtkImageGradient> grad = 
				vtkSmartPointer<vtkImageGradient>::New();
			grad->SetInputConnection(comps->GetOutputPort());
			grad->SetDimensionality(3);
			grad->Update();
			gradients[i] = grad->GetOutput();
			gradients[i]->Register(0);
			assert(gradients[i]->GetScalarType() == VTK_DOUBLE);
			gfield[i] = static_cast<double*>(gradients[i]->GetScalarPointer());
		}

		assert(volume->GetScalarType() == VTK_FLOAT);
		float *field = static_cast<float*>(volume->GetScalarPointer());
		int comps = volume->GetNumberOfScalarComponents();

		std::cout << "Building matrix..." << std::endl;
		data.set_size(width * height * depth, 12);
		for (int row = 0; row < width * height * depth; ++row)
		{
			NQVTK::Vector3 v(field[0], field[1], field[2]);
			field += comps;

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

			// Derivatives
			NQVTK::Vector3 ddx(gfield[0][0], gfield[1][0], gfield[2][0]);
			NQVTK::Vector3 ddy(gfield[0][1], gfield[1][1], gfield[2][1]);
			NQVTK::Vector3 ddz(gfield[0][2], gfield[1][2], gfield[2][2]);
			NQVTK::Matrix3x3 jacobian = NQVTK::Matrix3x3::fromCols(
				ddx, ddy, ddz);
			gfield[0] += 3;
			gfield[1] += 3;
			gfield[2] += 3;

			// Growth
			double detJ = std::abs(jacobian.det());
			if (detJ > 1.0)
			{
				data(row, 7) = 1.0 - (1.0 / detJ);
			}
			else
			{
				data(row, 7) = detJ - 1.0;
			}

			// Divergence
			data(row, 8) = jacobian.trace();

			// Curl
			data(row, 9) = jacobian.a21 - jacobian.a12;
			data(row, 10) = jacobian.a02 - jacobian.a20;
			data(row, 11) = jacobian.a10 - jacobian.a01;
		}

		// Clean up gradients
		for (int i = 0; i < 3; ++i)
		{
			gradients[i]->Delete();
		}
	}
}
