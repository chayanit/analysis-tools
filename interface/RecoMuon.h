#ifndef Analysis_Tools_RecoMuon_h
#define Analysis_Tools_RecoMuon_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      RecoMuon
// 
/**\class xxx RecoMuon.cc Analysis/Tools/src/RecoMuon.cc

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

      class RecoMuon : public Candidate {
         public:
            /// default constructor
            RecoMuon();
            /// constructor from 4-momentum information
            RecoMuon(const float & pt, const float & eta, const float & phi, const float & e, const float & q);
            /// destructor
           ~RecoMuon();
            // Gets

            // Sets
          protected:
          // ----------member data ---------------------------

         private:
         // ----------member data ---------------------------
            
      };
   }
}

#endif  // Analysis_Tools_RecoMuon_h
