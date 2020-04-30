// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/L1TMuon.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
L1TMuon::L1TMuon() : Candidate()
{
}
L1TMuon::L1TMuon(const float & pt, const float & eta, const float & phi, const float & e, const float & q) :
  Candidate(pt,eta,phi,e,q)
{
}
L1TMuon::~L1TMuon()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 

// pat muons
int    L1TMuon::hwQual()     const { return hwQual_  ; }
float  L1TMuon::etaAtVtx()   const { return etaAtVtx_; }
float  L1TMuon::phiAtVtx()   const { return phiAtVtx_; }

// Sets

void L1TMuon::hwQual   (const int   & hwQual)    { hwQual_   = hwQual  ;  }
void L1TMuon::etaAtVtx (const float & etaAtVtx)  { etaAtVtx_ = etaAtVtx;  }
void L1TMuon::phiAtVtx (const float & phiAtVtx)  { phiAtVtx_ = phiAtVtx;  }


// ------------ methods  ------------
