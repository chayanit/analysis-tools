#include "Analysis/Tools/interface/Analyser.h"

// this example is a dijet analysis in which the leading jet
// is required to have a muon, and both of the leading
// jets are btagged

using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   Analyser analyser(argc,argv);
   
   // you can always get the ususal analysis class
   auto analysis = analyser.analysis();
   
// HISTOGRAMS   
   // create some predefined jet histograms
   analyser.jetHistograms(2,"jets");
   analyser.jetHistograms(2,"btag_jets");
   
   for ( int i = 0 ; i < analyser.nEvents() ; ++i )
   {
      if ( ! analyser.event(i)                  )   continue;
      
// JETS pre-selection 
      // jet identification selection
      if ( ! analyser.selectionJetId()          )   continue;
      if ( ! analyser.selectionJetPileupId()    )   continue;
      if ( ! analyser.selectionNJets()          )   continue;
      
// CORRECTIONS to pre-selected jets
   // b energy regression
      if ( analyser.config()->bRegression() )
         analyser.actionApplyBjetRegression();
      
   // MC-only jet corrections
      if ( analyser.config()->isMC() )
      {
      // apply btag SF
         analyser.actionApplyBtagSF(1);
         analyser.actionApplyBtagSF(2);
         
      // jet energy resolution to all selected jets
         analyser.actionApplyJER();
      }
      
// MAIN SELECTION
   // JETS
      //  1st and 2nd jet kinematic selection, pt and eta
      if ( ! analyser.selectionJet(1)          )   continue;
      if ( ! analyser.selectionJet(2)          )   continue;
      // delta R jet selection
      if ( ! analyser.selectionJetDphi(1,2)    )   continue;
      if ( ! analyser.selectionJetDr(1,2)      )   continue;
// HISTOGRAMS
      analyser.fillJetHistograms("jets");
      // btag of two leading jets
      if ( ! analyser.selectionBJet(1)         )   continue;
      if ( ! analyser.selectionBJet(2)         )   continue;
// HISTOGRAMS
      analyser.fillJetHistograms("btag_jets");
      
      
   }  //end event loop
} // end main
      
