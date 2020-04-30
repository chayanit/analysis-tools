#ifndef Analysis_Tools_RecoTrack_h
#define Analysis_Tools_RecoTrack_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      RecoTrack
// 
/**\class xxx RecoTrack.cc Analysis/Tools/src/RecoTrack.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Fri, 25 May 2018 16:00:00 GMT
//
//

// system include files
#include <memory>
// 
// user include files
#include "Analysis/Tools/interface/Candidate.h"
//
// class declaration
//

namespace analysis {
   namespace tools {

      /// track quality
      enum TrackQuality {
          undefQuality = -1,
          loose = 0,
          tight = 1,
          highPurity = 2,
          confirmed = 3,  // means found by more than one iteration
          goodIterative = 4,  // meaningless
          looseSetWithPV = 5,
          highPuritySetWithPV = 6,
          discarded = 7, // because a better track found. kept in the collection for reference....
          qualitySize = 8
      };
      class RecoTrack : public Candidate {
         public:
            /// default constructor
            RecoTrack();
            /// constructor from 3-momentum information
            RecoTrack(const float & px, const float & py, const float & pz, const float & q);
            /// destructor
           ~RecoTrack();
            // Gets
            float chi2() const;
            float ndof() const;
            float d0() const;
            float dxy() const;
            int   numberOfLostMuonHits() const;
            int   numberOfBadMuonHits() const;
            int   numberOfValidMuonHits() const;
            int   numberOfValidTrackerHits() const;
            int   numberOfValidStripTECHits() const;
            int   numberOfValidStripTIBHits() const;
            int   numberOfValidStripTIDHits() const;
            int   numberOfValidStripTOBHits() const;
            int   muonStationsWithValidHits() const;
            int   muonStationsWithBadHits() const;
            int   innermostMuonStationWithValidHits() const;
            int   outermostMuonStationWithValidHits() const;
            bool  quality(const TrackQuality & trkqual);
            
            // Sets
            void chi2(const float &);
            void ndof(const float &);
            void d0(const float &);
            void dxy(const float &);
            void numberOfLostMuonHits(const int &);
            void numberOfBadMuonHits(const int &);
            void numberOfValidMuonHits(const int &);
            void numberOfValidTrackerHits(const int &);
            void numberOfValidStripTECHits(const int &);
            void numberOfValidStripTIBHits(const int &);
            void numberOfValidStripTIDHits(const int &);
            void numberOfValidStripTOBHits(const int &);
            void muonStationsWithValidHits(const int &);
            void muonStationsWithBadHits(const int &);
            void innermostMuonStationWithValidHits(const int &);
            void outermostMuonStationWithValidHits(const int &);
            void quality(const TrackQuality & , const bool &);
            
            
          protected:
          // ----------member data ---------------------------
          float chi2_;
          float ndof_;
          float d0_;
          float dxy_;
          int   nLostMuHits_;
          int   nBadMuHits_;
          int   nValMuHits_;
          int   nValTrackerHits_;
          int   nValStripTECHits_;
          int   nValStripTIBHits_;
          int   nValStripTIDHits_;
          int   nValStripTOBHits_;
          int   muStationsWithValHits_;
          int   muStationsWithBadHits_;
          int   inMuStationWithValHits_;
          int   outMuStationWithValHits_;
          std::map<TrackQuality,bool> qual_;

         private:
         // ----------member data ---------------------------
            
      };
   }
}

#endif  // Analysis_Tools_RecoTrack_h
