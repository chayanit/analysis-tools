#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"
#include "Analysis/Tools/bin/macro_config.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;

float btagMin(const std::string & btagwp);

// =============================================================================================   
int main(int argc, char * argv[])
{
   if ( macro_config(argc, argv) != 0 ) return -1;

   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   Analysis analysis(inputlist_);
   
   // btag
//   float btagmin = btagMin(btagwp_);
   // b-tag scale factors
   // inputs from the config file test/analysis_bjetsv2.cfg
   // btagalgo_ = "deepcsv";
   // btagsf_   = "../data/DeepCSV_94XSF_V3_B_F.csv";
   // btagwp_   = "tight";
   auto bsf_reader = analysis.btagCalibration(btagalgo_, btagsf_, btagwp_);
   
   // jer
   // Jet energy resolution scale factors and pt resolution
   auto jerinfo = analysis.jetResolutionInfo(jerpt_,jersf_);
   
   // Physics Objects Collections
   analysis.addTree<Jet> ("Jets",jetsCol_);
   analysis.addTree<GenParticle> ("GenParticles",genParticleCol_);
   analysis.addTree<GenJet> ("GenJets",genjetsCol_);
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevts = analysis.size();
   if ( nevts > 0 ) nevts = nevtmax_;
   for ( int i = 0 ; i < nevts ; ++i )
   {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i;
      
      // EventInfo
      std::cout << ", Run = " << analysis.run();
      std::cout << ", Event = " << analysis.event();
      std::cout << ", LumiSection = " << analysis.lumiSection();
      std::cout << "\n" << std::endl;      
      
      // GenParticles
      auto particles = analysis.collection<GenParticle>("GenParticles");  // of course one can also loop over the particles
      // GenJets
      auto genjets = analysis.collection<GenJet>("GenJets");              // of course one can also loop over the genjets
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      
      // associate partons to jets
      jets->associatePartons(particles,0.4,1);
      // associate genjets to jets
      jets->addGenJets(genjets);  // if not defined -> jerMatch = false, smearing will be done using the stochastic method only

      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         
      // BTAGGING
         float btag = -10000.;
         if ( btagalgo_ == "deepcsv" )
            btag = jet.btag("btag_deepb") + jet.btag("btag_deepbb");
         if ( btagalgo_ == "deepflavour" )
            btag = jet.btag("btag_dfb") + jet.btag("btag_dfbb") + jet.btag("btag_dflepb");
         
         // b-tag scale factors central, up and down
         double jet_bscalefactor     = jet.btagSF(bsf_reader); // OR jet.btagSF(analysis.btagCalibration()); 
         double jet_bscalefactorup   = jet.btagSFup(bsf_reader,2); 
         double jet_bscalefactordown = jet.btagSFdown(bsf_reader,2); 
         
      // JER 
         jet.jerInfo(*jerinfo,0.2); // this also performs matching to the added gen jets above, with delta R < 0.2 which is default and can be omitted
        
         
      // PRINTOUT
     
         std::cout << "Jet #" << j << ": ";
         std::cout << "pT  = "     << jet.pt()      << ", ";
         std::cout << "eta = "     << jet.eta()     << ", ";
         std::cout << "phi = "     << jet.phi()     << ", ";
         std::cout << "flavour = " << jet.flavour() << " (extended = " << jet.extendedFlavour() << "), ";
         std::cout << "btag = "    << btag    << " with scale factor = " <<  jet_bscalefactor;
         std::cout << " up  = "    << jet_bscalefactorup << "  down = " << jet_bscalefactordown << std::endl;
         std::cout << "     JER SF  = "    << jet.jerSF()  << ", match = " << jet.jerMatch() << " jer corr = " << jet.jerCorrection() << " + ";
         std::cout << jet.jerCorrection("up") << " - " << jet.jerCorrection("down") << std::endl;
         std::cout << "     quark-gluon likelihood = " << jet.qgLikelihood() << std::endl;
         std::cout << "     pileup jet id full discriminant = " << jet.pileupJetIdFullDiscriminant() << std::endl;
         std::cout << "     pileup jet id full id = " << jet.pileupJetIdFullId() << std::endl;
         
      }
      std::cout << "===================" << std::endl;
   }
   std::cout << "Btag WP = " << btagwp_ << std::endl;
   
//    
}

float btagMin(const string & wp)
{
   float min = -1000;
   if ( wp == "loose" )  min = btagwploose_;
   if ( wp == "medium" ) min = btagwpmedium_;
   if ( wp == "tight" )  min = btagwptight_;
   
   return min;
   
}

// ====================================================================
// // Below is a snippet of how to use the BTV standalone classes
// 
// // before the event loop
//    // b-tag scale factors
//    BTagCalibration calib(btagalgo_, btagsf_ );
//    BTagCalibrationReader reader(BTagEntry::OP_TIGHT,   // operating point - example for TIGHT
//                                 "central",             // central sys type
//                                 {"up", "down"});       // other sys types
//    reader.load(calib,             // calibration instance
//             BTagEntry::FLAV_B,    // btag flavour - B
//             "comb");              // measurement type   
//    reader.load(calib,             // calibration instance
//             BTagEntry::FLAV_C,    // btag flavour - C
//             "comb");              // measurement type   
//    reader.load(calib,             // calibration instance
//             BTagEntry::FLAV_UDSG, // btag flavour - UDSG
//             "incl");              // measurement type   
//    
// // inside the jet loop
//    
//          // b-tag scale factors
//          double jet_bscalefactor;
//          if ( jet.flavour("Hadron") == 5 ) jet_bscalefactor = reader.eval_auto_bounds("central", BTagEntry::FLAV_B,    fabs(jet.eta()), jet.pt() ); 
//          if ( jet.flavour("Hadron") == 4 ) jet_bscalefactor = reader.eval_auto_bounds("central", BTagEntry::FLAV_C,    fabs(jet.eta()), jet.pt() ); 
//          if ( jet.flavour("Hadron") == 0 ) jet_bscalefactor = reader.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, fabs(jet.eta()), jet.pt() ); 
//    

// ====================================================================


// // Below is a snippet of how to use the JME standalone classes
// 
// // before the event loop
//    // Jet energy resolution scale factors and pt resolution
//    JME::JetResolution resolution = JME::JetResolution(jerpt_);
//    JME::JetResolutionScaleFactor resolution_sf = JME::JetResolutionScaleFactor(jersf_);
// 
// // inside the event loop
//       analysis.match<Jet,GenJet>("Jets","GenJets",0.2);
// // inside the jet loop      
//          JME::JetParameters jetResPars = {{JME::Binning::JetPt, jet.pt()}, {JME::Binning::JetEta, jet.eta()}, {JME::Binning::Rho, jet.rho()}};
//          JME::JetParameters jetResSFPars = {{JME::Binning::JetEta, jet.eta()}, {JME::Binning::Rho, jet.rho()}};;
//          
//          std::cout << "resolution = " << resolution.getResolution(jetResPars) << ", ";
//          std::cout << "JER SF  = "    << resolution_sf.getScaleFactor(jetResSFPars)  << ", " << genjet << std::endl;
//          
