
#include "VLoop.h"

VLoop::VLoop( VParameters *irunpara  )
{

  // Run Parameters
  fRunPara = irunpara;

  // Randoms
  int seed = time(NULL)*getpid();
  rand = new TRandom3(seed);

  // Outout root file
  //fOutPutFile = new TFile(fRunPara->sOutPutFileName.c_str(),"RECREATE");

}

void VLoop::analyse()
{

  cout << "Number of Situations " << pow(2,fRunPara->iNumberOfIssues) << endl;
  int iSit = pow(2,fRunPara->iNumberOfIssues);

  vector< vector<int> > iSituations;
  vector<int> itemp;

  for( int i = 0; i < iSit; i++ )
  {

    for( int j = 0; j < fRunPara->iNumberOfIssues; j++ )
      itemp.push_back( convertDectoBin(i,j) );

    iSituations.push_back(itemp);
    itemp.resize(0);

  }

  for( int i = 0; i < iSituations.size(); i++ )
  {
    cout << "Sit " << i+1 << "\t";
    for( int j = 0; j < iSituations[i].size(); j++ )
      cout << iSituations[i][j] << " ";
    cout << endl;
  }

  cout << endl;
  cout << "Number of Rules " << fRunPara->iNumberOfRules << endl;

  for( int i = 0; i < fRunPara->iNumberOfRules; i++ )
  {
    cout << fRunPara->sParties[fRunPara->iRules[i][0]] << " ";
    for( int j = 1; j < fRunPara->iRules[i].size(); j++ )
      cout << fRunPara->iRules[i][j] << " ";
    cout << endl;
  }

  vector< vector<int> > iTable;
  vector<int> jtemp;

  for( int i = 0; i < iSit; i++ )
  //for( int i = 0; i < fRunPara->iNumberOfRules; i++ )
  {
    for( int j = 0; j < fRunPara->iNumberOfRules; j++ )
    //for( int j = 0; j < iSit; j++ )
      jtemp.push_back( applyRule(iSituations[i],fRunPara->iRules[j]) ); 
      //jtemp.push_back( applyRule(iSituations[j],fRunPara->iRules[i]) ); 
    iTable.push_back( jtemp );
    jtemp.resize(0);
  }

  cout << endl;
  for( unsigned int i = 0; i < iTable.size(); i++ )
  {
    cout << "Sit " << i+1 << "\t";
    for( unsigned int j = 0; j < iTable[i].size(); j++ )
      cout << iTable[i][j] << " " ;
    cout << endl;
  }

  cout << endl;
  cout << "Min Invasive Situation    " <<  minInvasive( iTable, fRunPara->iRules ) << endl;
  cout << "Max Alog Situation        " <<  maxAlgo( iTable, fRunPara->iRules ) << endl;
  cout << "Compromise Algo Situation " <<  compAlgo( iTable, fRunPara->iRules ) << endl;
  cout << endl;

  return;

}

int VLoop::convertDectoBin( int i, int j )
{

  if( i ==0 ) return 0;

  vector<int> temp;
  int ftemp = i;

  while( ftemp >= 1 )
  {
    if( ftemp%2 == 0 ) 
      temp.push_back( 0 );
    else
      temp.push_back( 1 );
    ftemp = int(ftemp/2.);
  }

  if( j >= temp.size() ) return 0;

  return temp[j];

}

int VLoop::applyRule( vector<int> iSit, vector<int> iRule )
{

// Rule 1 
  if( iRule[1] == 1 )
  {
    if( iSit[iRule[2]-1] == 1 ) 
      return 1;
    else
      return 0;
  }

// Rule 2 
  if( iRule[1] == 2 )
  {
    if( iSit[iRule[2]-1] == 0 ) 
      return 1;
    else
      return 0;
  }

// Rule 3 
  if( iRule[1] == 3 )
  {
    if( iSit[iRule[2]-1] == 1 && iSit[iRule[3]-1] == 1) 
      return 1;
    else
      return 0;
  }

// Rule 4 
  if( iRule[1] == 4 )
  {
    if( iSit[iRule[2]-1] == 1 || iSit[iRule[3]-1] == 1) 
      return 1;
    else
      return 0;
  }

// Rule 5 
  if( iRule[1] == 5 )
  {
    if( iSit[iRule[2]-1] == 1 || iSit[iRule[3]-1] == 1) 
      return 1;
    else
      return 0;
  }


  return -1;

}

int VLoop::minInvasive( vector< vector<int> > iTable, vector< vector<int> > iRule )
{

  int vs;
  int vs_max = -999999;
  int ivs_max = -99;

  for( int i = 0; i < iTable.size(); i++ )
  {

    vs = 0;
    for( int j = 0; j < iTable[i].size(); j++ )
      vs += iTable[i][j]*iRule[j][4];

    if( vs > vs_max )
    {
      vs_max = vs;
      ivs_max = i;
    }

  }

  return ivs_max+1;

}


int VLoop::maxAlgo( vector< vector<int> > iTable, vector< vector<int> > iRule )
{

  int vs;
  vector<int> temp;
  vector< vector<int> > va;
  
  for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
  {
     for( int i = 0; i < iTable.size(); i++ )
     {
       vs = 0;
       for( int j = 0; j < iTable[i].size(); j++ )
       {
         if( k == iRule[j][0] ) vs += iTable[i][j]*iRule[j][4];
       }
       temp.push_back( vs );
     }
     va.push_back( temp );
     temp.resize(0);
  }

  vector<int> mu;
  for( int i = 0; i < iTable[0].size(); i++ )
  {
    int min = 99999999;
    for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
    {
      if( va[k][i] < min ) min = va[k][i];
    }
    mu.push_back( min );
  }

  int imax = -99;
  int max = -9999999999;
  for( int i = 0; i < mu.size(); i++ )
  {
    if( mu[i] > max )
    { 
      max = mu[i]; 
      imax = i;
    }
  }

  return imax + 1;

}

int VLoop::compAlgo( vector< vector<int> > iTable, vector< vector<int> > iRule )
{


  int vs;
  vector<int> temp;
  vector< vector<int> > va;
  
  for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
  {
     for( int i = 0; i < iTable.size(); i++ )
     {
       vs = 0;
       for( int j = 0; j < iTable[i].size(); j++ )
       {
         if( k == iRule[j][0] ) vs += iTable[i][j]*iRule[j][4];
       }
       temp.push_back( vs );
     }
     va.push_back( temp );
     temp.resize(0);
  }

  vector<int> mu;
  for( int i = 0; i < iTable[0].size(); i++ )
  {
    int min = 99999999;
    int max = -99999999;
    for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
    {
      if( va[k][i] < min ) min = va[k][i];
      if( va[k][i] > max ) max = va[k][i];
    }
    mu.push_back( max - min );
  }

  int imin = -99;
  int min = 9999999999;
  for( int i = 0; i < mu.size(); i++ )
  {
    if( mu[i] < min )
    { 
      min = mu[i]; 
      imin = i;
    }
  }

  return imin + 1;


}


