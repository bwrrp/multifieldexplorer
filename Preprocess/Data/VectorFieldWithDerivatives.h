#pragma once

#include "Field.h"

namespace PropertySpace
{
	class VectorFieldWithDerivatives : public Field
	{
	public:
		static VectorFieldWithDerivatives *Load(const std::string &filename);

	protected:
		VectorFieldWithDerivatives();

		void BuildPropertyMatrix(vtkImageData *volume);
	};
}
