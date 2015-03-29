#ifndef VParameter_H
#define VParameter_H

#include <TDatime.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TSystem.h>

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string>

#include <VGlobalRunParameter.h>

using namespace std;

class VParameters
{

  private:

  public:

// Input Parameters:
	VParameters();
	~VParameters() {}

	void print();
	void initialise();

// Input Filenames
        string fInPutFile;

	vector<string> sParties;
	vector<string> sIssues;
	vector<string> sRules;

	vector< vector<int> > iRules;

	int iNumberOfParties;
	int iNumberOfIssues;
	int iNumberOfRules;

};

#endif
