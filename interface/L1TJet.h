#ifndef Analysis_Tools_L1TJet_h
#define Analysis_Tools_L1TJet_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      L1TJet
// 
/**\class xxx L1TJet.cc Analysis/Tools/src/L1TJet.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
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

      class L1TJet : public Candidate {
         public:
            /// default constructor
            L1TJet();
	    /// constructor from 4-momentum information
            L1TJet(const float & pt, const float & eta, const float & phi, const float & e, const float & q);
	    /// destructor
           ~L1TJet();
//            using Candidate::set; // in case needed to overload the function set
        	   // Gets
        
        	   // Sets
   
          protected:
       // ----------member data ---------------------------

         private:
            // ----------member data ---------------------------
            
            // 
      };
   }
}

#endif  // Analysis_Tools_Jet_h
