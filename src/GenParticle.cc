// system include files
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/GenParticle.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
GenParticle::GenParticle() : Candidate() 
{
}
GenParticle::GenParticle(const float & pt, const float & eta, const float & phi, const float & e, const float & q) : 
      Candidate(pt,eta,phi,e,q) 
{
   indx_ = -1;
   mo_[0] = -1;
   mo_[1] = -1;
   da_[0] = -1;
   da_[1] = -1;
   
}
GenParticle::~GenParticle()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
