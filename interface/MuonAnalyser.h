#ifndef Analysis_Tools_MuonAnalyser_h
#define Analysis_Tools_MuonAnalyser_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      xxx
// 
/**\class xxx MuonAnalyser.cc Analysis/Tools/src/MuonAnalyser.cc

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
#include "Analysis/Tools/interface/BaseAnalyser.h"
//
// class declaration
//

namespace analysis {
   namespace tools {

      class MuonAnalyser  : virtual public analysis::tools::BaseAnalyser {
         
         public:
            /// constructors
            MuonAnalyser();
            MuonAnalyser(int argc, char * argv[]);
            /// desctructor
           ~MuonAnalyser();
           
            // ----------member data ---------------------------
         protected:
            std::vector< std::shared_ptr<Muon> > muons_;
            std::vector< std::shared_ptr<Muon> > selectedMuons_;
            std::vector< std::shared_ptr<Muon> > onlineMatchedMuons_;
            
            bool muonsanalysis_;
            
            std::map<std::string, std::shared_ptr<BTagCalibrationReader> >bsf_reader_;
         
         private:
               
         public:
            std::vector< std::shared_ptr<Muon> > selectedMuons();
            std::vector< std::shared_ptr<Muon> > onlineMatchedMuons();
            std::vector< std::shared_ptr<Muon> > muons();
            float btag(const Muon & , const std::string & );
         
            // Sets
            
            // Actions
            virtual bool analysisWithMuons();
            virtual bool selectionMuon(const int &);
            virtual bool selectionMuons();
//            virtual bool selectionMuon();
            virtual bool selectionMuonId();
            virtual bool selectionNMuons();
            virtual bool onlineMuonMatching();
            virtual bool onlineL1MuonMatching(const int &);
            virtual bool onlineL3MuonMatching(const int &);
            virtual void muonHistograms(const std::string &, const int & n = 1);
            virtual void fillMuonHistograms();

      };
   }
}

#endif  // Analysis_Tools_MuonAnalyser_h
