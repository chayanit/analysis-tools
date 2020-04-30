#ifndef Analysis_Tools_JetAnalyser_h
#define Analysis_Tools_JetAnalyser_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      JetAnalyser
// 
/**\class xxx JetAnalyser.h Analysis/Tools/interface/JetAnalyser.h

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

      class JetAnalyser  : virtual public analysis::tools::BaseAnalyser {
         
         public:
            /// constructors
            JetAnalyser();
            JetAnalyser(int argc, char * argv[]);
            /// desctructor
           ~JetAnalyser();
           
            // ----------member data ---------------------------
         protected:
            std::vector< std::shared_ptr<Jet> > jets_;
            std::vector< std::shared_ptr<Jet> > selectedJets_;
            
            // number of histogrammed jets
            int n_hjets_;
            
            bool jetsanalysis_;
            bool applyjer_;
            
            std::map<std::string, std::shared_ptr<BTagCalibrationReader> >bsf_reader_;
            
            std::shared_ptr<JetResolutionInfo> jerinfo_;
            
            std::vector<std::string> flavours_;
         
         private:
               
         public:
            /// vector of pointers of the selectedJets
            std::vector< std::shared_ptr<Jet> > selectedJets();
            /// vector of pointers of all jets from the "Jets" collection
            std::vector< std::shared_ptr<Jet> > jets();
            
            /// Returns the btag value of the jet for a given btag algorithm
            float btag(const Jet & jet, const std::string & algo);
         
            /// Creates the "Jets" collection
            void jets(const std::string & col);
            
            // Actions
            virtual bool analysisWithJets();
            /// Given the ranking 'r' of a jet, it returns whether the jet passes the pt_min and |eta_max|, optionally pt_max, where the values of the thresholds  pt_min and |eta_max|, pt_max are passed by configuration file
            virtual bool selectionJet(const int & r);
            /// Given the ranking 'r' of a jet, it returns whether the jet passes the pt_min and |eta_max|, optionally pt_max, where the values of the thresholds  pt_min and |eta_max|, pt_max (opt) are passed as arguments
            virtual bool selectionJet(const int & r, const float & pt_min, const float &eta_max, const float &pt_max=-1. );
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_eta selection; the threshold delta is passed by the configuration file
            virtual bool selectionJetDeta(const int & r1, const int &r2);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_eta selection; the threshold delta is passed as an argument (if delta <= 0, |delta| = delta_min; else |delta| = delta_max )
            virtual bool selectionJetDeta(const int & r1, const int & r2, const float & delta);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_phi selection; the threshold delta is passed by the configuration file
            virtual bool selectionJetDphi(const int & r1, const int & r2);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_phi selection; the threshold delta is passed as an argument (if delta <= 0, |delta| = delta_min; else |delta| = delta_max )
            virtual bool selectionJetDphi(const int & r1, const int & r2, const float & delta);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_R selection; the threshold delta is passed by the configuration file
            virtual bool selectionJetDr(const int & r1, const int & r2);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the delta_R selection; the threshold delta is passed as an argument (if delta <= 0, |delta| = delta_min; else |delta| = delta_max )
            virtual bool selectionJetDr(const int & r1, const int & r2, const float & delta);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the pt imbalance selection; the threshold delta is passed by the configuration file
            virtual bool selectionJetPtImbalance(const int & r1, const int & r2);
            /// Given the rankings r1 and r2 of two jets, it returns whether the jets passes the pt imbalance selection; the threshold delta is passed as an argument (if delta <= 0, |delta| = delta_min; else |delta| = delta_max )
            virtual bool selectionJetPtImbalance(const int & r1, const int & r2, const float & delta);
            
            
            virtual bool selectionJetId();
            virtual bool selectionJetPileupId();
            virtual bool selectionNJets();
            virtual bool selectionDiJetMass(const int &, const int &);
            virtual bool selectionBJet(const int &);
            virtual bool selectionBJetProbB(const int &);
            virtual bool selectionBJetProbBB(const int &);
            virtual bool selectionBJetProbLepB(const int &);
            virtual bool selectionBJetProbC(const int &);
            virtual bool selectionBJetProbG(const int &);
            virtual bool selectionBJetProbLight(const int &);
            virtual ScaleFactors btagSF(const int &, const std::string &);
            virtual bool selectionNonBJet(const int &);
            virtual bool onlineJetMatching(const int &);
            virtual bool onlineBJetMatching(const int &);
            virtual void jetHistograms(const int & n, const std::string & label = "x");
            virtual void jetHistograms(const std::string & label = "x");
            virtual void fillJetHistograms(const std::string & label = "x");
            virtual void fillJetHistograms(const int & r = -1, const std::string & label = "x", const float & sf = 1.,const bool & workflow = false);
            virtual void actionApplyJER();
            virtual float actionApplyBtagSF(const int &, const bool & global_weight = true);
            virtual float getBtagSF(const int &);
            virtual void actionApplyBjetRegression();
            virtual void jetSwap(const int &, const int &);
            virtual bool selectionJetQGlikelihood(const int &, const float &);
            virtual bool selectionJetQGlikelihood(const int &);
//            virtual void actionApplyBtagEfficiencyWeight(const int &);
//            virtual float btagEfficiency(const int &);

      };
   }
}

#endif  // Analysis_Tools_JetAnalyser_h
