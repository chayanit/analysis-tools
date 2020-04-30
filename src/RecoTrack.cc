// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/RecoTrack.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
RecoTrack::RecoTrack() : Candidate()
{
}
RecoTrack::RecoTrack(const float & px, const float & py, const float & pz, const float & q) :
  Candidate(px,py,pz,q)
{
}
RecoTrack::~RecoTrack()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 
float RecoTrack::chi2()                                const { return chi2_;                    }
float RecoTrack::ndof()                                const { return ndof_;                    }
float RecoTrack::d0()                                  const { return d0_;                      }
float RecoTrack::dxy()                                 const { return dxy_;                     }
int   RecoTrack::numberOfLostMuonHits()                const { return nLostMuHits_;             }
int   RecoTrack::numberOfBadMuonHits()                 const { return nBadMuHits_;              }
int   RecoTrack::numberOfValidMuonHits()               const { return nValMuHits_;              }
int   RecoTrack::numberOfValidTrackerHits()            const { return nValTrackerHits_;         }
int   RecoTrack::numberOfValidStripTECHits()           const { return nValStripTECHits_;        }
int   RecoTrack::numberOfValidStripTIBHits()           const { return nValStripTIBHits_;        }
int   RecoTrack::numberOfValidStripTIDHits()           const { return nValStripTIDHits_;        }
int   RecoTrack::numberOfValidStripTOBHits()           const { return nValStripTOBHits_;        }
int   RecoTrack::muonStationsWithValidHits()           const { return muStationsWithValHits_;   }
int   RecoTrack::muonStationsWithBadHits()             const { return muStationsWithBadHits_;   }
int   RecoTrack::innermostMuonStationWithValidHits()   const { return inMuStationWithValHits_;  }
int   RecoTrack::outermostMuonStationWithValidHits()   const { return outMuStationWithValHits_; }
bool  RecoTrack::quality(const TrackQuality & trkqual) { return qual_[trkqual];           }

// Sets
void RecoTrack::chi2(const float & chi2)                          { chi2_ = chi2;                 }
void RecoTrack::ndof(const float & ndof)                          { ndof_ = ndof;                 }
void RecoTrack::d0(const float & d0  )                            { d0_   = d0;                   }
void RecoTrack::dxy(const float & dxy )                           { dxy_  = dxy;                  }
void RecoTrack::numberOfLostMuonHits(const int & n)               { nLostMuHits_ = n;             }
void RecoTrack::numberOfBadMuonHits(const int & n)                { nBadMuHits_ = n;              }
void RecoTrack::numberOfValidMuonHits(const int & n)              { nValMuHits_ = n;              }
void RecoTrack::numberOfValidTrackerHits(const int & n)           { nValTrackerHits_ = n;         }
void RecoTrack::numberOfValidStripTECHits(const int & n)          { nValStripTECHits_ = n;        }
void RecoTrack::numberOfValidStripTIBHits(const int & n)          { nValStripTIBHits_ = n;        }
void RecoTrack::numberOfValidStripTIDHits(const int & n)          { nValStripTIDHits_ = n;        }
void RecoTrack::numberOfValidStripTOBHits(const int & n)          { nValStripTOBHits_ = n;        }
void RecoTrack::muonStationsWithValidHits(const int & n)          { muStationsWithValHits_ = n;   }
void RecoTrack::muonStationsWithBadHits(const int & n)            { muStationsWithBadHits_ = n;   }
void RecoTrack::innermostMuonStationWithValidHits(const int & n)  { inMuStationWithValHits_ = n;  }
void RecoTrack::outermostMuonStationWithValidHits(const int & n)  { outMuStationWithValHits_ = n; }
void RecoTrack::quality(const TrackQuality & trkqual, const bool & status)  {  qual_[trkqual] = status; }


