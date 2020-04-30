#include "Analysis/Tools/interface/Analyser.h"

// this example has no selection
// for MC it will apply the generator weights

using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   Analyser analyser(argc,argv);
   auto config = analyser.config();
   
// HISTOGRAMS definitions  
   // create some predefined jet histograms
   // if not defined, the number of jets is nJetMin from the configurations
   analyser.jetHistograms("selection01");
   
   for ( int i = 0 ; i < analyser.nEvents() ; ++i )
   {
      if ( ! analyser.event(i)                  )   continue;
      
   // JETS
      analyser.actionApplyJER();                               // correction : jet energy resolution smearing
      analyser.actionApplyBjetRegression();                    // correction : jet energy regression (for b jets)
      if ( ! analyser.selectionJetId()          )   continue;  // selection  : jet identification 
      if ( ! analyser.selectionJetPileupId()    )   continue;  // selection  : jet Pileup identification 
      if ( ! analyser.selectionNJets()          )   continue;  // selection  : number of jets 
      
      // the lines below will only be executed if rank <= nJetsMin
      if ( ! analyser.selectionJet(1)           )   continue;  // selection  : jet1 pt and eta 
      if ( ! analyser.selectionJet(2)           )   continue;  // selection  : jet2 pt and eta 
      if ( ! analyser.selectionJetDphi(1,2)     )   continue;  // selection  : delta_phi_jets (1,2) [or  MIN(neg): analyser.selectionJetDphi(1,2,-2.0) / MAX(pos): analyser.selectionJetDphi(1,2,+2.0)]
      
   // HISTOGRAMS
      analyser.fillJetHistograms("selection01");               // histograms : jets fill
      
   }  //end event loop
   

} // end main
      
