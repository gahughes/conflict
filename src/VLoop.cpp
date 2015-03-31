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

  for( unsigned int i = 0; i < iSituations.size(); i++ )
  {
    cout << "Situations " << i+1 << "\t";
    for( unsigned int j = 0; j < iSituations[i].size(); j++ )
      cout << iSituations[i][j] << " ";
    cout << endl;
  }

// Where the MAGIC happens
  fillTables( iSituations );
  doCuts_temp();

  cout << endl;
  cout << "Number of Issues " << fRunPara->iNumberOfIssues << endl;

  for( int i = 0; i < fRunPara->iNumberOfIssues; i++ )
  {
    cout << fRunPara->sIssues[i].c_str() << " ";
    cout << endl;
  }

  cout << endl;
  cout << "Number of Rules " << fRunPara->iNumberOfRules << endl;

  for( int i = 0; i < fRunPara->iNumberOfRules; i++ )
  {
    cout << fRunPara->sParties[fRunPara->iRules[i][0]] << " ";
    cout << fRunPara->sRules[i].c_str() << " ";
    cout << endl;
  }

/*
  vector< vector<int> > iTable;
  vector<int> jtemp;
  for( int i = 0; i < iSit; i++ )
  {
    for( int j = 0; j < fRunPara->iNumberOfRules; j++ )
      jtemp.push_back( applyRule(iSituations[i],fRunPara->iRules[j]) ); 
    iTable.push_back( jtemp );
    jtemp.resize(0);
  }
*/

  cout << endl;
  for( unsigned int i = 0; i < fRunPara->vTable.size(); i++ )
  {
    cout << "Results: " << i+1 << "\t";
    for( unsigned int j = 0; j < fRunPara->vTable[i].size(); j++ )
      cout << fRunPara->vTable[i][j] << " " ;
    cout << endl;
  }

  vector<int> minOut = minInvasive( fRunPara->vTable, fRunPara->iRules );
  vector<int> maxOut = maxAlgo( fRunPara->vTable, fRunPara->iRules );
  vector<int> compOut = compAlgo( fRunPara->vTable, fRunPara->iRules );

  cout << endl;
  cout << "Min Invasive Situation    ";
  for( unsigned int i = 0; i < minOut.size(); i++ ) 
    cout << minOut[i] << " ";
  cout << endl;
  cout << "Max Alog Situation        ";
  for( unsigned int i = 0; i < maxOut.size(); i++ ) 
    cout << maxOut[i] << " ";
  cout << endl;
  cout << "Compromise Algo Situation ";
  for( unsigned int i = 0; i < compOut.size(); i++ ) 
    cout << compOut[i] << " ";
  cout << endl;

/*
//TEMPTEMP 
  cout << endl;
  cout << endl;
  for( unsigned int j = 0; j < iSituations[0].size(); j++ )
  {
    //cout << "Sit " << "\t";
    for( unsigned int i = 0; i < iSituations.size(); i++ )
      cout << iSituations[i][j] << " ";
    cout << endl;
  }
  cout << endl;

  for( unsigned int j = 0; j < fRunPara->vTable[0].size(); j++ )
  {
    for( unsigned int i = 0; i < fRunPara->vTable.size(); i++ )
      cout << fRunPara->vTable[i][j] << " " ;
    cout << endl;
  }
*/

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

  if( j >= (int)temp.size() ) return 0;

  return temp[j];

}

/*
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
*/

//int VLoop::minInvasive( vector< vector<Long64_t> > iTable, vector< vector<int> > iRule )
vector<int> VLoop::minInvasive( vector< vector<Long64_t> > iTable, vector< vector<int> > iRule )
{

  vector<int> out;
  int vs;
  int vs_max = -99999999;

  for( unsigned int i = 0; i < iTable.size(); i++ )
  {
    vs = 0;
    for( unsigned int j = 0; j < iTable[i].size(); j++ )
      vs += iTable[i][j]*iRule[j][1];
    if( vs >= vs_max )
      vs_max = vs;
  }

  for( unsigned int i = 0; i < iTable.size(); i++ )
  {
    vs = 0;
    for( unsigned int j = 0; j < iTable[i].size(); j++ )
      vs += iTable[i][j]*iRule[j][1];
    if( vs == vs_max )
      out.push_back(i+1);
  }

  return out;

}


//int VLoop::maxAlgo( vector< vector<Long64_t> > iTable, vector< vector<int> > iRule )
vector<int> VLoop::maxAlgo( vector< vector<Long64_t> > iTable, vector< vector<int> > iRule )
{

  vector<int> out;
  int vs;
  vector<int> temp;
  vector< vector<int> > va;
  
  for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
  {
     for( unsigned int i = 0; i < iTable.size(); i++ )
     {
       vs = 0;
       for( unsigned int j = 0; j < iTable[i].size(); j++ )
       {
         if( k == iRule[j][0] ) vs += iTable[i][j]*iRule[j][1];
       }
       temp.push_back( vs );
     }
     va.push_back( temp );
     temp.resize(0);
  }

  vector<int> mu;
  for( unsigned int i = 0; i < iTable.size(); i++ )
  {
    int min = 99999999;
    for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
    {
      if( va[k][i] < min ) min = va[k][i];
    }
    mu.push_back( min );
  }

  int imax = -99;
  int mmax = -99999999;
  for( unsigned int i = 0; i < mu.size(); i++ )
  {
    if( mu[i] >= mmax )
    { 
      mmax = mu[i]; 
      imax = i;
    }
  }

  for( unsigned int i = 0; i < mu.size(); i++ )
  {
    if( mu[i] == mmax )
      out.push_back(i+1);
  }

  return out;

}

vector<int> VLoop::compAlgo( vector< vector<Long64_t> > iTable, vector< vector<int> > iRule )
{

  vector<int> out;
  int vs;
  vector<int> temp;
  vector< vector<int> > va;
  
  for( int k = 0; k < fRunPara->iNumberOfParties; k++ )
  {
     for( unsigned int i = 0; i < iTable.size(); i++ )
     {
       vs = 0;
       for( unsigned int j = 0; j < iTable[i].size(); j++ )
       {
         if( k == iRule[j][0] ) vs += iTable[i][j]*iRule[j][1];
       }
       temp.push_back( vs );
     }
     va.push_back( temp );
     temp.resize(0);
  }

  vector<int> mu;
  //for( unsigned int i = 0; i < iTable[0].size(); i++ )
  for( unsigned int i = 0; i < iTable.size(); i++ )
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
  int mmin = 99999999;
  for( unsigned int i = 0; i < mu.size(); i++ )
  {
    if( mu[i] <= mmin )
    { 
      mmin = mu[i]; 
      imin = i;
    }
  }

  for( unsigned int i = 0; i < mu.size(); i++ )
    if( mu[i] == mmin )
      out.push_back(i+1);

  return out;


}

void VLoop::fillTables( vector< vector<int> > iSituations )
{

  TFile *file = new TFile("temp.root","RECREATE");

  int x[100];

  for( unsigned int i = 0; i < iSituations.size(); i++ )
  {
    char sSitName[100];
    sprintf(sSitName,"S%d",i);
    fRunPara->tRules.push_back( new TTree(sSitName,sSitName) );
  }

  for( unsigned int i = 0; i < iSituations.size(); i++ )
  {
    for( unsigned int j = 0; j < iSituations[i].size(); j++ )
    {
      x[j] = iSituations[i][j];
      char sBranchName[100];
      char sBranchName2[100];
      sprintf(sBranchName,"%s",fRunPara->sAlphabet[j].c_str());
      sprintf(sBranchName2,"%s/I",fRunPara->sAlphabet[j].c_str());
      fRunPara->tRules[i]->Branch(sBranchName,&x[j],sBranchName2);
    }
    fRunPara->tRules[i]->Fill();
    fRunPara->tRules[i]->Write();
  }


  file->Close();

  return;

}

void VLoop::doCuts()
{

  vector<Long64_t> vtemp;

  for( unsigned int i = 0; i < fRunPara->tRules.size(); i++ )
  {
    char sSitName[100];
    sprintf(sSitName,"S%d",i);
    for( unsigned int j = 0; j < fRunPara->sRules.size(); j++ )
    {
      vtemp.push_back( fRunPara->tRules[i]->Draw("a",fRunPara->sRules[j].c_str(),"goff") );
      //cout << "xx " << xx << endl; 
      //cout << i << " " << j << " " << fRunPara->tRules[i]->Draw("a",fRunPara->sRules[j].c_str(),"goff") << endl;
      //int xx = int(fRunPara->tRules[i]->Draw("a",fRunPara->sRules[j].c_str(),"goff"));
      //cout << i << " ";
      //cout << j << " ";
      //cout << xx << " ";
    }
    fRunPara->vTable.push_back( vtemp );
    vtemp.resize(0);
  }

  return;

}

void VLoop::doCuts_temp()
{

  TFile *tempfile = TFile::Open("temp.root");

  vector<Long64_t> vtemp;

  for( unsigned int i = 0; i < fRunPara->tRules.size(); i++ )
  {
    char sSitName[100];
    sprintf(sSitName,"S%d",i);

TTree *tree = (TTree*)tempfile->Get(sSitName);

    for( unsigned int j = 0; j < fRunPara->sRules.size(); j++ )
    {
      vtemp.push_back( tree->Draw("a",fRunPara->sRules[j].c_str(),"goff") );
    }
    fRunPara->vTable.push_back( vtemp );
    vtemp.resize(0);

delete tree;

  }

  return;

}
