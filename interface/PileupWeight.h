#ifndef Analysis_Tools_PileupWeight_h
#define Analysis_Tools_PileupWeight_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      xxx
// 
/**\class xxx PileupWeight.cc Analysis/Tools/src/PileupWeight.cc

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
#include <vector>
#include <string>
#include <map>
#include <iostream>
// 
// user include files

#include "TH1.h"


//
// class declaration
//

namespace analysis {
   namespace tools {

      class PileupWeight {
         public:
            /// constructors
            PileupWeight();
            PileupWeight(const std::string &);
            /// desctructor
           ~PileupWeight();
           
            // ----------member data ---------------------------
         protected:
            std::map<int,std::shared_ptr<TH1D> > histos_;
                        
         private:
               
         public:
            float weight(const float & truepu, const int & var = 0);
            std::shared_ptr<TH1D> histogram(const int & var = 0);
      };
   }
}

#endif  // Analysis_Tools_PileupWeight_h
