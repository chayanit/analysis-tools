// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/L1TJet.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
L1TJet::L1TJet() : Candidate()
{
}
L1TJet::L1TJet(const float & pt, const float & eta, const float & phi, const float & e, const float & q) :
  Candidate(pt,eta,phi,e,q)
{
}
L1TJet::~L1TJet()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 


// Sets


// ------------ methods  ------------
