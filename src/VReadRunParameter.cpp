#include <VReadRunParameter.h>

VReadRunParameter::VReadRunParameter()
{

    fDebug = false;
    if( fDebug ) cout << "VReadRunParameter::VReadRunParameter()" << endl;
    fRunPara = new VParameters();

}

// Read the command line args
bool VReadRunParameter::readCommandline( int argc, char *argv[] )
{

    int i = 1;
// no command line parameters
    if( argc == 1 )
    {
        printHelp();
        return false;
    }
// read all command line parameters
    while( i++ < argc )
    {
        string iTemp  = argv[i-1];
        string iTemp1 = argv[i-1];                // this is to get the camera name correctly
        string iTemp2 = "";
        if( i < argc )
        {
            iTemp2 = argv[i];
        }
// print help text
        if( iTemp.find( "help" ) < iTemp.size() )
        {
            printHelp();
            return false;
        }

	if( iTemp.find( "inputfile"  ) < iTemp.size() )
	{
	    fRunPara->fInPutFile = iTemp.substr( iTemp.rfind( "=" )+1, iTemp.size() ).c_str();
	} 

    }

   if( fRunPara->fInPutFile == "" )
   {
     cout << "Input File needed " << fRunPara->fInPutFile.c_str() << endl;
     cout << "Exiting ... " << endl;
     exit(-1);
   }

   printStartMessage();

   return true;

}

void VReadRunParameter::printHelp()
{

	printStartMessage();
	int temp = system("cat ../README/README.commandline");
	temp *= 1;

}

void VReadRunParameter::printStartMessage()
{

  cout << endl;
  cout << "A Model of Conflict Resolution" << endl;
  cout << " " << endl;
  cout << "\tFrieder Lempp" << endl;
  cout << "\tMassey University" << endl;
  cout << "\tF.Lempp@massey.ac.nz" << endl;
  cout << " " << endl;
  cout << " " << endl;
  cout << "Code by: " << endl;
  cout << "\tGareth Hughes" << endl;
  cout << "\tETH Zurich" << endl;
  cout << "\tghughes@ohys.ethz.ch" << endl;
  cout << " " << endl;
  cout << endl;

}


// Read the XRay data
void VReadRunParameter::readInPutFile( string fInfile )
{

  vector<string> sInPuts;

  string line;
  string temp;

  ifstream myfile;
  myfile.open ( fInfile.c_str() );

  if (myfile.is_open())
  {
    while ( getline(myfile,line) )
    {
      istringstream stream( line );
      sInPuts.push_back( line );
    }
    myfile.close();
  }
  else cout << "Unable to open file " << fInfile.c_str() << endl;; 

  myfile.close();

  unsigned int iNum = atoi(sInPuts[0].c_str());
  for( unsigned int i = 1; i < iNum+1; i++ )
    fRunPara->sParties.push_back(sInPuts[i].c_str());

  unsigned int iNum2 = atoi(sInPuts[iNum+1].c_str());
  for( unsigned int i = iNum+2; i < iNum+iNum2+2; i++ )
    fRunPara->sIssues.push_back(sInPuts[i].c_str());

  vector<string> tokens;
  vector<int> jtemp;
  int iNextIndex = iNum+iNum2+2;
  unsigned int iNum3 = atoi(sInPuts[iNextIndex].c_str());

  iNextIndex++;
  for( unsigned int i = iNextIndex; i < iNextIndex+iNum3; i++ )
  {
    istringstream iss(sInPuts[i]);
    copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(tokens));
    for( unsigned int j = 0; j < tokens.size(); j++ )
      jtemp.push_back(atoi(tokens[j].c_str()));
    fRunPara->iRules.push_back(jtemp);
    jtemp.resize(0);
    tokens.resize(0);
  }

  iNextIndex += iNum3;
  for( unsigned int i = iNextIndex; i < iNextIndex+iNum3; i++ )
    fRunPara->sRules.push_back(sInPuts[i].c_str());

  fRunPara->iNumberOfParties = iNum;
  fRunPara->iNumberOfIssues  = iNum2;
  fRunPara->iNumberOfRules   = iNum3;

  fillAlphabet();

}

void VReadRunParameter::fillAlphabet()
{

 fRunPara->sAlphabet.push_back("a"); 
 fRunPara->sAlphabet.push_back("b"); 
 fRunPara->sAlphabet.push_back("c"); 
 fRunPara->sAlphabet.push_back("d"); 
 fRunPara->sAlphabet.push_back("e"); 
 fRunPara->sAlphabet.push_back("f"); 
 fRunPara->sAlphabet.push_back("g"); 
 fRunPara->sAlphabet.push_back("h"); 
 fRunPara->sAlphabet.push_back("i"); 
 fRunPara->sAlphabet.push_back("j"); 
 fRunPara->sAlphabet.push_back("k"); 
 fRunPara->sAlphabet.push_back("l"); 
 fRunPara->sAlphabet.push_back("m"); 
 fRunPara->sAlphabet.push_back("n"); 
 fRunPara->sAlphabet.push_back("o"); 
 fRunPara->sAlphabet.push_back("p"); 
 fRunPara->sAlphabet.push_back("q"); 
 fRunPara->sAlphabet.push_back("u"); 
 fRunPara->sAlphabet.push_back("r"); 
 fRunPara->sAlphabet.push_back("s"); 
 fRunPara->sAlphabet.push_back("t"); 
 fRunPara->sAlphabet.push_back("v"); 
 fRunPara->sAlphabet.push_back("w"); 
 fRunPara->sAlphabet.push_back("x"); 
 fRunPara->sAlphabet.push_back("y"); 
 fRunPara->sAlphabet.push_back("z"); 

}

