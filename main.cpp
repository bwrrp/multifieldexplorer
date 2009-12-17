#include "Data/Field.h"
#include "Data/VectorFieldWithDerivatives.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
	//typedef PropertySpace::Field FieldType;
	typedef PropertySpace::VectorFieldWithDerivatives FieldType;

	if (argc < 2)
	{
		std::cerr << "Specify field to load on the command line" << std::endl;
		return 1;
	}

	if (argc < 3)
	{
		std::cerr << "Specify output filename on the command line" << 
			std::endl;
		return 1;
	}

	int numComps = 0;
	if (argc >= 4)
	{
		numComps = atoi(argv[3]);
	}

	// Load the field
	std::cout << "Loading field..." << std::endl;
	FieldType *field = FieldType::Load(argv[1]);
	if (!field)
	{
		std::cerr << "Error loading field" << std::endl;
		return 1;
	}

	// Do PCA
	field->DoPCA();

	// Transform extended field to normalized PCA basis
	field->Transform();

	// Save transformed field as volumes
	field->Save(argv[2], numComps);

	// Clean up
	delete field;

#ifndef NDEBUG
	// Keep the console window open for testing
	std::cout << "Press any enter to exit" << std::endl;
	getchar();
#endif
	return 0;
}
