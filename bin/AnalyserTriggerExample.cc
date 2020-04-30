#include "Analysis/Tools/interface/Analyser.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   Analyser trigger(argc,argv);
   
   auto analysis = trigger.analysis(); // that's the usual Analysis class

   for ( int i = 0 ; i < trigger.nEvents() ; ++i )
   {
      if ( ! trigger.event(i) ) continue;
      // trigger selection
      if ( ! trigger.selectionHLT() )   continue;
      if ( ! trigger.selectionL1 () )   continue;  // to be used in case of "OR" of seeds

      // One can always use the usual Analysis to retrieve some information
      auto l1jets_ana = analysis->collection<TriggerObject>(trigger.config()->triggerObjectsL1Jets());
      for ( int j = 0 ; j < l1jets_ana->size() ; ++j )
      {
         TriggerObject l1jet = l1jets_ana->at(j);
         std::cout << "L1 Jet (Analysis): pT = " << l1jet.pt() << ", " << l1jet.eta() << ", " << l1jet.phi() << std::endl;
      }
      std::cout << "----" << std::endl;
      
      // But sometimes there is a method in the analyser one can use  directly    
      // L1 jets trigger objects
      auto l1jets = trigger.triggerObjectsL1Jets();
      for ( auto jet : l1jets )
      {
         std::cout << "L1 Jet (Analyser): pT = " << jet->pt() << ", " << jet->eta() << ", " << jet->phi() << std::endl;
      }
      std::cout << "====" << std::endl;
   }
   
   return 0;
}
