// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/RecoMuon.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
RecoMuon::RecoMuon() : Candidate()
{
}
RecoMuon::RecoMuon(const float & pt, const float & eta, const float & phi, const float & e, const float & q) :
  Candidate(pt,eta,phi,e,q)
{
}
RecoMuon::~RecoMuon()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 

// Sets


