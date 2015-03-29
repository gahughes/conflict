#include "VParameters.h"

VParameters::VParameters()
{

	fInPutFile = "";
	iNumberOfParties = 0;
	iNumberOfIssues = 0;


}

void VParameters::initialise()
{

   

}

void VParameters::print()
{

	cout << "Input File:         " << fInPutFile.c_str() << endl;
	cout << endl;

}
