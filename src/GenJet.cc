// system include files
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/GenJet.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
GenJet::GenJet() : Candidate() 
{
}
GenJet::GenJet(const float & pt, const float & eta, const float & phi, const float & e, const float & q) : 
      Candidate(pt,eta,phi,e,q) 
{
}
GenJet::~GenJet()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
