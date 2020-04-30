#ifndef Analysis_Tools_Composite_h
#define Analysis_Tools_Composite_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Composite
// 
/**\class xxx Composite.h Analysis/Tools/interface/Composite.h

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
      

      template <class A, class B>
      class Composite : public Candidate {
         public:
            /// default constructor
            Composite();
            /// constructor from other objects
            Composite(const A &, const B &);
            /// destructor
           ~Composite();
//            using Candidate::set; // in case needed to overload the function set
           // Gets
           A * objecA();
           B * objecB();
           float deltaR() const;
           float deltaEta() const ;
           
           float btag(const std::string & ) const;

           // Sets
           void objectA(const A &);
           void objectB(const B &);
           

       protected:
       // ----------member data ---------------------------

       private:
           // ----------member data ---------------------------
            
           A const * a_;
           B const * b_;
             
             
            // 
      };
   }
}

#endif  // Analysis_Tools_Composite_h
