#ifndef Analysis_Tools_Analyser_h
#define Analysis_Tools_Analyser_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      xxx
// 
/**\class xxx Analyser.cc Analysis/Tools/src/Analyser.cc

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
// 
// user include files
#include "Analysis/Tools/interface/TriggerAnalyser.h"
#include "Analysis/Tools/interface/JetAnalyser.h"
#include "Analysis/Tools/interface/MuonAnalyser.h"


//
// class declaration
//

namespace analysis {
   namespace tools {

      class Analyser : 
            public analysis::tools::TriggerAnalyser,
            public analysis::tools::JetAnalyser,
            public analysis::tools::MuonAnalyser

      {
         
         public:
            /// constructors
            Analyser();
            Analyser(int argc, char * argv[]);
            /// desctructor
           ~Analyser();
           
         
            // ----------member data ---------------------------
         protected:
         
         private:
            
         public:
            // Actions
            virtual bool event(const int &);
         
            void generatorWeight();
               
            virtual bool muonJet(const int & );

      };
   }
}

#endif  // Analysis_Tools_Analyser_h
