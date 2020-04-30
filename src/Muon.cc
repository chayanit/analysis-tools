// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/Muon.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Muon::Muon() : Candidate()
{
}
Muon::Muon(const float & pt, const float & eta, const float & phi, const float & e, const float & q) :
  Candidate(pt,eta,phi,e,q)
{
}
Muon::~Muon()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 

// pat muons
bool  Muon::isPFMuon()                             const { return isPFMuon_;                }
bool  Muon::isGlobalMuon()                         const { return isGlobalMuon_;            }
bool  Muon::isTrackerMuon()                        const { return isTrackerMuon_;           }
bool  Muon::isLooseMuon()                          const { return isLooseMuon_;             }
bool  Muon::isMediumMuon()                         const { return isMediumMuon_;            }
bool  Muon::isTightMuon()                          const { return isTightMuon_;             }
bool  Muon::id(const std::string & id ) const
{
   std::string lid = id;
   std::transform(lid.begin(), lid.end(), lid.begin(), ::tolower);
   if ( lid == "tight"  ) return isTightMuon_ ;
   if ( lid == "medium" ) return isMediumMuon_;
   if ( lid == "loose"  ) return isLooseMuon_ ;
   
   std::cout << "*** warning *** Muon::id -> id type " << id << " is invalid. Returning false!" << std::endl;
   return false; 
}

// Inner tracker vars 
float Muon::validFraction()                        const { return validFraction_;           }
float Muon::segmentCompatibility()                 const { return segmentCompatibility_;    }
float Muon::trkKink()                              const { return trkKink_;                 }
float Muon::chi2LocalPos()                         const { return chi2LocalPos_;            }

// Global tracker vars
float Muon::normChi2()                             const { return normChi2_;                }


// Sets

void Muon::isPFMuon     (const bool & isPFMuon)       { isPFMuon_      = isPFMuon;       }
void Muon::isGlobalMuon (const bool & isGlobalMuon)   { isGlobalMuon_  = isGlobalMuon;   }
void Muon::isTrackerMuon(const bool & isTrackerMuon)  { isTrackerMuon_ = isTrackerMuon;  }
void Muon::isLooseMuon  (const bool & isLooseMuon)    { isLooseMuon_   = isLooseMuon;    }
void Muon::isMediumMuon (const bool & isMediumMuon)   { isMediumMuon_  = isMediumMuon;   }
void Muon::isTightMuon  (const bool & isTightMuon)    { isTightMuon_   = isTightMuon;    }

void Muon::validFraction( const float & validFraction)                   { validFraction_        = validFraction ;       }
void Muon::segmentCompatibility(const float & segmentCompatibility)      { segmentCompatibility_ = segmentCompatibility ;}
void Muon::trkKink      ( const float & trkKink     )                    { trkKink_      = trkKink;         }
void Muon::chi2LocalPos ( const float & chi2LocalPos)                    { chi2LocalPos_ = chi2LocalPos;    }

void Muon::normChi2     ( const float & normChi2 )                       {  normChi2_ = normChi2 ;          }


// ------------ methods  ------------
//void Muon::id (){}

