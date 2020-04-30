#ifndef Analysis_Tools_GenParticle_h
#define Analysis_Tools_GenParticle_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      GenParticle
// 
/**\class xxx GenParticle.cc Analysis/Tools/src/GenParticle.cc

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

      class GenParticle : public Candidate {
         public:
            GenParticle();
            GenParticle(const float & pt, const float & eta, const float & phi, const float & e, const float & q);
           ~GenParticle();
//            using Candidate::set; // in case needed to overload the function set
            void pdgId(const int & pdgId);
            void status(const int & status);
            void higgsDaughter(const bool & higgs_dau);
            void index(const int &);
            void mother(const int &, const int &);
            void daughter(const int &, const int &);
            
            int pdgId();
            int status();
            bool higgsDaughter();
            int index();
            int mother(const int & n = 1);
            int daughter(const int & n = 1);
      
         private:
            // ----------member data ---------------------------
            int   pdgid_;
            int   status_;
            bool  higgs_dau_;
            int   indx_;
            int   mo_[2];
            int   da_[2];
            // 
      };
      // ===============================================
      // INLINE IMPLEMENTATIONS
         
      // Gets
      inline int   GenParticle::pdgId()                    { return pdgid_    ; }                   
      inline int   GenParticle::status()                   { return status_   ; }                   
      inline bool  GenParticle::higgsDaughter()            { return higgs_dau_; }                   
      // parenthood
      inline int   GenParticle::index()                    { return indx_     ; }
      inline int   GenParticle::mother(const int & n)
      {
         if ( n < 1 || n > 2 ) { std::cout << "*w* GenParticle has two possible mothers"   << std::endl; return -1; }
         return mo_[n-1]    ;
      }
      inline int   GenParticle::daughter(const int & n)
      {
         if ( n < 1 || n > 2 ) { std::cout << "*w* GenParticle has two possible daughters" << std::endl; return -1; }
         return da_[n-1]    ;
      }
      // Sets                                                             
      inline void GenParticle::pdgId  (const int & pdgId)             { pdgid_  = pdgId  ; } 
      inline void GenParticle::status (const int & status)            { status_ = status ; } 
      inline void GenParticle::higgsDaughter (const bool & higgs_dau) { higgs_dau_ = higgs_dau; } 
      // parenthood
      inline void GenParticle::index(const int & indx)                { indx_ = indx    ; } 
      inline void GenParticle::mother(const int & n, const int & mo)
      {
         if ( n < 1 || n > 2 ) { std::cout << "*w* GenParticle has two possible mothers" << std::endl; }
         else  mo_[n-1] = mo  ; 
      }
      inline void GenParticle::daughter(const int & n, const int & da)
      {
         if ( n < 1 || n > 2 ) { std::cout << "*w* GenParticle has two possible daughters" << std::endl; }
         else  da_[n-1] = da  ; 
      }
         
   }
}

#endif  // Analysis_Tools_GenParticle_h
