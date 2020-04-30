#ifndef Analysis_Tools_MuonIdWeight_h
#define Analysis_Tools_MuonIdWeight_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      xxx
// 
/**\class xxx MuonIdWeight.cc Analysis/Tools/src/MuonIdWeight.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Antonio Vagnerini
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

#include "TH2.h"


//
// class declaration
//

namespace analysis {
   namespace tools {

      class MuonIdWeight {
         public:
            /// constructors
            MuonIdWeight();
            MuonIdWeight(const std::string &);
            /// desctructor
           ~MuonIdWeight();
           
            // ----------member data ---------------------------
         protected:
            std::map<int,std::shared_ptr<TH2D> > histos_;
                        
         private:
               
         public:
            float weight(const float & pt, const float & eta, const int & var = 0);
            std::shared_ptr<TH2D> histogram(const int & var = 0);
      };
   }
}

#endif  // Analysis_Tools_MuonIdWeight_h
