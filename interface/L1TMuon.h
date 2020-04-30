#ifndef Analysis_Tools_L1TMuon_h
#define Analysis_Tools_L1TMuon_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      L1TMuon
// 
/**\class xxx L1TMuon.cc Analysis/Tools/src/L1TMuon.cc

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

      class L1TMuon : public Candidate {
         public:
            /// default constructor
            L1TMuon();
	    /// constructor from 4-momentum information
            L1TMuon(const float & pt, const float & eta, const float & phi, const float & e, const float & q);
	    /// destructor
           ~L1TMuon();
//            using Candidate::set; // in case needed to overload the function set
        	   // Gets
        	   int   hwQual()     const ;
        	   float etaAtVtx()   const ; 
        	   float phiAtVtx()   const ; 
        
        	   // Sets
        	   void hwQual  (const int & hwQual);    
            void etaAtVtx(const float & etaAtVtx); 
            void phiAtVtx(const float & phiAtVtx); 
   
          protected:
       // ----------member data ---------------------------
            int   hwQual_;             
            float etaAtVtx_;         
            float phiAtVtx_;        

         private:
            // ----------member data ---------------------------
            
            // 
      };
   }
}

#endif  // Analysis_Tools_Muon_h
