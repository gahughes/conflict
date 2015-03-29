#ifndef VSEGMENTLOOP_H
#define VSEGMENTLOOP_H

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <math.h>
#include <TRandom3.h>

#include <VParameters.h>
#include <VGlobalRunParameter.h>

#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"



using namespace std;

class VLoop : VParameters
{

  private:
	TRandom3 *rand;
	VParameters *fRunPara; 		// run parameters

	//TFile *fOutPutFile; 		// output root file

  public:

	VLoop( VParameters* );
	virtual ~VLoop() {}

	void analyse();
	int convertDectoBin(int i, int j);
	int applyRule( vector<int> iSit, vector<int> iRule );
	int minInvasive( vector< vector<int> > iTable, vector< vector<int> > iRule );
	int maxAlgo( vector< vector<int> > iTable, vector< vector<int> > iRule );
	int compAlgo( vector< vector<int> > iTable, vector< vector<int> > iRule );

};

#endif 

